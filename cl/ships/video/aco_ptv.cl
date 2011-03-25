/*
 * 	aco_ptv.cl
 *
 *  Created on	: Mar 23, 2011
 *  Author		: Phan Quoc Huy (diepvien00thayh@gmail.com)
 * 
 */
 
#pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_local_int32_base_atomics : enable
#pragma OPENCL EXTENSION cl_khr_global_int32_extended_atomics : enable

#define		ALPHA 		3.0
#define 	BETA 		5.0
#define 	RHO			0.5
#define		TAU0		5.0
#define		CLUSTER_MAX	12

__constant sampler_t imageSampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST; 

/*
 * @brief calculate the relaxation length from p0 to p1
 * @param clusters0
 * @param clusters0
 * @param f0
 * @param f1
 * @param p0 id of first particle
 * @param p1 id of second particle
 * @param cluster_size size of cluster.
 * @return the relaxation length from p0 to p1
 */

float relaxation_length_(

__read_only image2d_t 	clusters0, 
__read_only image2d_t 	clusters1, 
__constant 	float2* 	f0, 
__constant 	float2* 	f1, 
const 		uint 		p0, 
const 		uint 		p1, 
const 		uint 		cluster_size
){
	uint i,j,mid;
	int4 ir1, ir2, temp;
	float mi, diff, len;
	float2 r1, r2, d1, d2;
	bool tabu[CLUSTER_MAX];

	for(i = 0;i<CLUSTER_MAX;i++)
		tabu[i] = true;
		
	d1 = f0[p0];
	d2 = f1[p1];
	
	for(i=0;i<cluster_size;i++){
		ir1 = read_imagei(clusters0, imageSampler, (int2)(p0,i+1));
		r1 = f0[ir1.x];
		mi = INT_MAX;
		
		for (j = 0; j < cluster_size; j++) {
			if (tabu[j] == true) {
				temp = read_imagei(clusters1, imageSampler, (int2) (p1, j + 1));
				r2 = f1[temp.x];
				diff = sqrt((d2.x + r1.x - d1.x - r2.x) * (d2.x + r1.x - d1.x - r2.x) + (d2.y + r1.y - d1.y - r2.y) * (d2.y + r1.y - d1.y - r2.y));

				if (diff < mi ) {
					mi = diff;
					ir2 = temp;
					mid = j;
				}
			}
		}		
		tabu[mid] = false;
		len += mi;
	}
	return len;
};

/*
 * @brief purpose of this kernel is to test the relaxation length calculation function.
 * @param clusters0
 * @param clusters0
 * @param f0
 * @param f1
 * @param result output value.
 * @param p0 id of first particle
 * @param p1 id of second particle
 * @param cluster_size size of cluster.
 */

__kernel void relaxation_length(

__read_only image2d_t 	clusters0, 
__read_only image2d_t 	clusters1, 
__constant 	float2* 	f0, 
__constant 	float2* 	f1, 
__global	float*		result, //output
const 		uint 		p0, 
const 		uint 		p1, 
const 		uint 		cluster_size)
{
	result[0] = relaxation_length_(clusters0, clusters1, f0, f1, p0, p1, cluster_size);
};

/*
 * @brief perform one step of n loops.
 * @param clusters0
 * @param clusters1
 * @param clusters01 stores ordered closest particles to the row_id'th particle.
 * @param cf0f1
 * @param Tk1 If one thread matches a particle to another, Tk1's length = num_particles. \
 * If one thread calculates probability from one particle to onther in clusters01, \
 * Tk1's length = num_particles * cluster01_max
 * @param f0
 * @param f1
 * @param init_pos
 * @param N
 * @param num_particles
 * @param num_ants
 * @param cluster01_max
 * @param cluster_max
 * @param cluster_size
 * 
 */ 

__kernel void one_step(

__read_only 	image2d_t 	clusters0, //0
__read_only 	image2d_t 	clusters1, //1
__read_only 	image2d_t 	clusters01, //2
__read_only 	image2d_t 	cf0f1, //3
__constant		float2*		f0, //4
__constant		float2*		f1, //5
__global		float*		tau, //6
__global		float*		Lk, //7
__global		int*		Tk0, //8
__global		int*		Tk1, //9 
__global 		int* 		init_pos, //10 
__global 		int* 		N, //11
const 			uint 		num_particles, //12
const 			uint 		num_ants, //13
const			uint		cluster01_max, //14
const			uint		cluster_max, //15
const			uint		cluster_size //16
){
	uint globalIdx = get_global_id(0);
	uint globalIdy = get_global_id(1);

	uint globalHeight = get_global_size(1);
	uint i, j;
	uint pid; //particle id 1st frame
	int4 pid1; //particle id 2nd frame
	
	float temp, tauij, temp1, prob = 1, eta = 0, denominator = 0;
	float ma[2]; //max probability
	float len[2]; //min length
	int   ima[2]; //max index
	
	for(i=0;i<num_ants;i+=globalHeight){ 
		//pid = (globalIdx + num_particles - init_pos[i]) % num_particles;
		pid = Tk0[i*num_particles + globalIdx];
		ma[1] = 1;
		for(j=0;j<cluster01_max;j++){
			pid1 = read_imagei(clusters01, imageSampler, (int2)(j, pid)); // pop a next-closest particle from cluster
			if(atom_cmpxchg(N,1,0)){ //check if it's unvisited
				temp1 = relaxation_length_(clusters0, clusters1, f0, f1, pid, pid1.x, cluster_size);
				eta = 1/temp1;
				tauij = tau[pid*num_particles + pid1.x];
				prob = (pow(tauij, ALPHA) * pow(eta, BETA));
				
				len[(int)(ma[1]<prob)] = temp1;
				ima[(int)(ma[1]<prob)] = pid1.x;
				ma[(int)(ma[1]<prob)] = prob;
			}
		}
		Lk[i*globalHeight + globalIdy] += len[1]; //accumulates the total length
		Tk1[i*num_particles + globalIdx] = ima[1]; //highest probability particle has been chosen.
	}
	
	barrier(CLK_GLOBAL_MEM_FENCE);
};

/*
 * @brief update the tau matrix 
 * @param tau tau matrix
 * @param Lk array of Lk values (sum of relaxation lengths)
 * @param num_ants number of ants to update the matrix
 * @param num_particles number of particles equal to the width of global NDrange
 */

__kernel void update(

__global 	float* 	tau, 
__global 	float* 	Lk, 
__global 	int* 	Tk0, 
__global 	int*	Tk1, 
const 		int 	num_ants,
const 		int 	num_particles
){
	uint globalHeight = get_global_size(1);
	uint globalIdx = get_global_id(0);
	uint globalIdy = get_global_id(1);
	
	float delta_tau;
	uint id;
	
	for(uint i=0;i<num_ants;i+=globalHeight){
		delta_tau = 1/Lk[i];
		id = (i + globalIdy)*num_particles + globalIdx;
		tau[Tk1[id] + Tk0[id]*num_particles]+=delta_tau;
	}
	barrier(CLK_GLOBAL_MEM_FENCE);
};

/*
 * @brief calculates the distances betweeen particles of f0 and f1 frames.
 * @param result the result matrix
 * @param f0 first frame
 * @param f1 second frame
 * @param size number of partciles
 */
 
__kernel void distances(

__write_only 	image2d_t 	result, 
__constant 		float2* 	f0, 
__constant 		float2* 	f1, 
const			uint 		size
){
	uint globalIdx = get_global_id(0);
	uint globalIdy = get_global_id(1);
	uint jump = 4096 / size;
	uint i = globalIdy;
	
	while(i<size){
		write_imagef(result, (int2) (globalIdx, i), (float4) distance(f1[globalIdx], f0[i]));
		i+=jump;
	}
};

/*
 * @brief group particles into clusters.
 * @param dis the distance matrix
 * @param clusters output clusters <output>
 * @param cluster in shared memory
 * @param cluster_size size of cluster
 * @param num_particles number of particles
 */
 
__kernel void clusters(

__read_only 	image2d_t 	dis, 
__write_only 	image2d_t	clusters, 
__local 		int* 		cluster, 
const			uint 		cluster_size, 
const			uint 		num_particles
){
	uint globalIdx = get_global_id(0);
	uint localIdx = get_local_id(0);
	
	float temp1=0.0f;
	float temp[]={0.0f,1000.0f};
	float4 elem;
	int itemp[2];	
	
	for(uint j=0;j<cluster_size;j++){
		temp[1] = 1000.0f;
		for(uint i=0;i<num_particles;i++){
			elem = read_imagef(dis, imageSampler, (int2)(i, globalIdx));
			itemp[(int)((temp[1] > elem.x) && (temp1 < elem.x))] = i;
			temp[(int)((temp[1] > elem.x) && (temp1 < elem.x))] = elem.x;
		}
		temp1 = temp[1];
		cluster[j + localIdx*cluster_size] = itemp[1];
	}
	barrier(CLK_LOCAL_MEM_FENCE);

	for(uint i=0;i<cluster_size;i++)
		write_imagei(clusters,(int2) (i, globalIdx), convert_int4(cluster[i + localIdx*cluster_size]));

	barrier(CLK_GLOBAL_MEM_FENCE);
}

__kernel void zeros(__write_only image2d_t clusters, const uint cluster_size){
	uint globalIdx = get_global_id(0);
	int group_size = get_local_size(0);

	int i;
	for(i=0;i<cluster_size;i++){
		write_imagei(clusters,(int2) (i, globalIdx), (int4) cluster_size);
	}
}
