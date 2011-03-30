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

#define		ALPHA 		3.0f
#define 	BETA 		5.0f
#define 	RHO			0.5f
#define		TAU0		5.0f
#define		CLUSTER_MAX	12
#define		NUM_THREADS	4096

__constant sampler_t imageSampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST; 

/*
 * @brief calculate the conventional length from p0 to p1
 * @param f0
 * @param f1
 * @param p0 id of first particle
 * @param p1 id of second particle
 * @return the length from p0 to p1
 */
 
inline float length_(

__constant 	float2* 	f0, 
__constant 	float2* 	f1, 
const 		uint 		p0, 
const 		uint 		p1
){
	return sqrt(pow(f0[p0].x - f1[p1].x,2) + pow(f0[p0].y - f1[p1].y,2));
};

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
const 		uint 		cluster_size //4 6 8
){
	uint i,j,mid;
	int4 ir1, ir2, temp;
	float mi, diff, len=0;
	float2 r1, r2, d1, d2;
	bool tabu[CLUSTER_MAX];

	for(i = 0;i<CLUSTER_MAX;i++)
		tabu[i] = true;
		
	d1 = f0[p0];
	d2 = f1[p1];
	
	for(i=0;i<cluster_size;i++){
		ir1 = read_imagei(clusters0, imageSampler, (int2)(i + 1, p0));
		r1 = f0[ir1.x];
		mi = INT_MAX;
		
		for (j = 0; j < cluster_size; j++) {
			if (tabu[j] == true) {
				temp = read_imagei(clusters1, imageSampler, (int2) (j + 1, p1));
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
	//len *= ((float) this->cluster_min_ / this->cluster_size_);
	return len;
};

/*
 * @brief purpose of this kernel is to test the relaxation length calculation.
 * @param clusters0
 * @param clusters0
 * @param f0
 * @param f1
 * @param result output value.
 * @param cluster_size size of cluster.
 */

__kernel void relaxation_length(

__read_only image2d_t 	clusters0, 
__read_only image2d_t 	clusters1, 
__constant 	float2* 	f0, 
__constant 	float2* 	f1, 
__global	float*		result, //output
const 		uint 		cluster_size
){
	uint gX = get_global_id(0);
	
	result[gX] = relaxation_length_(clusters0, clusters1, f0, f1, gX, gX, cluster_size);
};

/*
 * @brief perform one step of n loops.
 * @param dis
 * @param ids
 * @param tau
 * @param Lk
 * @param Tk0
 * @param Tk1 If one thread matches a particle to another, Tk1's length = num_particles. \
 * If one thread calculates probability from one particle to onther in clusters01, \
 * Tk1's length = num_particles * cluster01_max
 * @param N
 * @param num_particles
 * @param num_ants
 * @param cluster01_max
 * @param cluster_max
 * @param cluster_size
 */ 

__kernel void one_step(

__read_only 	image2d_t 	dis, 	//0
__read_only 	image2d_t 	ids, 	//1
__global		float*		tau, 	//2
__global		float*		Lk, 	//3
__global		int*		Tk0, 	//4
__global		int*		Tk1, 	//5 
__global 		int* 		N, 		//6
const 			uint 		num_particles, 	//7
const 			uint 		num_ants, 		//8
const			uint		cluster01_max, 	//9
const			uint		cluster_max, 	//10
const			uint		cluster_size 	//11
){
	uint gX = get_global_id(0);
	uint gY = get_global_id(1);

	uint gH = get_global_size(1);
	uint i, j, k;
	uint pid; //particle id 1st frame
	int4 pid1; //particle id 2nd frame
	
	float temp, tauij, prob = 1, eta = 0, denominator = 0;
	float ma[2]; //max probability
	float len[2]; //min length
	float4 temp1;
	int   ima[2]; //max index
	
	for(i=0;i<num_ants;i+=gH){ 
		pid = Tk0[(i + gY)*num_particles + gX];
		for(k=0;k<cluster01_max;k++){ //if the highest probability particle is occupied, then try again
			ma[1] = -1.0f;
			for(j=0;j<cluster01_max;j++){ //look into the cluster to find the highest probability particle
				pid1 = read_imagei(ids, imageSampler, (int2)(j, pid)); // pop a next-closest particle from cluster
				if(N[pid1.x + (i + gY) * num_particles] == 1){ //check if it's unvisited
					temp1 = read_imagef(dis, imageSampler, (int2)(j, pid));

					eta = 1/temp1.x;
					tauij = tau[pid*num_particles + pid1.x];
					prob = pow(tauij, ALPHA) * pow(eta, BETA);

					len[(int)(ma[1]<prob)] = temp1.x;
					ima[(int)(ma[1]<prob)] = pid1.x;
					ma[(int)(ma[1]<prob)] = prob;
				}
			}

			if(atom_cmpxchg((__global int*)&N[ima[1] + (i + gY) * num_particles],1,0)==1)
				break;
		}
		//atom_add((__global int*)Lk + i + gY, 4); //No float atomic add
		Lk[i + gY] += len[1]; //accumulates the total length

		barrier(CLK_GLOBAL_MEM_FENCE);
		Tk1[(i + gY)*num_particles + gX] = ima[1]; //highest probability particle has been chosen.
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
	uint gH = get_global_size(1);
	uint gX = get_global_id(0);
	uint gY = get_global_id(1);
	
	float delta_tau;
	uint id;
	
	for(uint i=0;i<num_ants;i+=gH){
		delta_tau = 1/Lk[i + gY];
		id = (i + gY) * num_particles + gX;
		tau[Tk1[id] + Tk0[id]*num_particles]+=delta_tau;
	}
	
	barrier(CLK_GLOBAL_MEM_FENCE);
};

/*
 * @brief update tau matrix with rate of evaporation RHO
 * @param tau tau matrix
 * @param num_particles
 */
  
__kernel void update_rho(

__global 	float* 	tau, 
const 		int 	num_particles
){
	uint gH = get_global_size(1);
	uint gX = get_global_id(0);
	uint gY = get_global_id(1);
	
	for(uint i=0;i<num_particles;i+=gH)
		tau[(i + gY) * num_particles + gX]*=(1-RHO);

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
	uint gX = get_global_id(0);
	uint gY = get_global_id(1);
	uint jump = 4096 / size;
	uint i = gY;
	
	while(i<size){
		write_imagef(result, (int2) (gX, i), (float4) distance(f1[gX], f0[i]));
		i+=jump;
	}
};

/*
 * @brief pre-calculate the relaxation distance between particles
 * @param result output
 * @param cf0 clustered frame 0
 * @param cf1
 * @param f0
 * @param f1
 * @param size number of particles
 */
  
__kernel void r_distances(

__write_only 	image2d_t 	result,
__read_only		image2d_t	clusters0,
__read_only		image2d_t	clusters1,
__read_only		image2d_t	clusters01,
__constant 		float2* 	f0, 
__constant 		float2* 	f1, 
const			uint 		num_particles,
const			uint 		cluster_size
){
	uint lX = get_local_id(0);
	uint lY = get_local_id(1);
	
	uint groupX = get_group_id(0);
	uint gH = get_global_size(1);
	uint numGroups0 = get_num_groups(0);
	
	uint i,j, id1, id2;
	float temp;
	int4 pid2;
	
	for(i=0;i<num_particles;i+=gH*numGroups0){
		id2 = lX;
		id1 = lY + groupX*gH + i;
		pid2 = read_imagei(clusters01,imageSampler, (int2)(id2 , id1));
		
		temp = relaxation_length_(clusters0, clusters1, f0, f1, id1, pid2.x, cluster_size);
		write_imagef(result, (int2)(id2, id1), convert_float4(temp));
	}
	
	barrier(CLK_GLOBAL_MEM_FENCE);
};

/*
 * @brief group particles into clusters.
 * @param dis the distance matrix
 * @param clusters output clusters <output>
 * @param icluster in shared memory
 * @param cluster in shared memory
 * @param cluster_size size of cluster
 * @param num_particles number of particles
 */
 
__kernel void clusters(

__read_only 	image2d_t 	dis, 
__write_only 	image2d_t	iclusters,
__local 		int* 		icluster,
const			uint 		cluster_size, 
const			uint 		num_particles
){
	uint gX = get_global_id(0);
	uint lX = get_local_id(0);
	
	float temp1=-1.0f;
	float temp[]={0.0f,1000.0f};
	float4 elem;
	int itemp[2];	
	
	for(uint j=0;j<cluster_size;j++){
		temp[1] = 1000.0f;
		for(uint i=0;i<num_particles;i++){
			elem = read_imagef(dis, imageSampler, (int2)(i, gX));
			itemp[(int)((temp[1] > elem.x) && (temp1 < elem.x))] = i;
			temp[(int)((temp[1] > elem.x) && (temp1 < elem.x))] = elem.x;
		}
		temp1 = temp[1];
		icluster[j + lX*cluster_size] = itemp[1];
	}
	barrier(CLK_LOCAL_MEM_FENCE);

	for(uint i=0;i<cluster_size;i++)
		write_imagei(iclusters,(int2) (i, gX), convert_int4(icluster[i + lX*cluster_size]));

	barrier(CLK_GLOBAL_MEM_FENCE);
}

__kernel void sort(

__read_only 	image2d_t 	dis, //distances
__read_only 	image2d_t	ids, //indices
__write_only 	image2d_t	new_dis,
__write_only	image2d_t	new_ids,
const			uint 		cluster_size
){
	uint gX = get_global_id(0);
	uint lX = get_local_id(0);
	
	float temp1=-1.0f;
	float temp[2];
	int itemp[2];
	
	float4 val;//value
	int4 id;//index
	
	for(uint j=0;j<cluster_size;j++){
		temp[1] = 1000.0f;
		for(uint i=0;i<cluster_size;i++){
			id = read_imagei(ids, imageSampler, (int2)(i, gX)); //read id of the particle in 2nd frame
			val = read_imagef(dis, imageSampler, (int2)(i, gX)); //and its distance from gX-th particle in 1st frame
			itemp[(int)((temp[1] > val.x) && (temp1 < val.x))] = id.x;
			temp[(int)((temp[1] > val.x) && (temp1 < val.x))] = val.x;
		}
		temp1 = temp[1];
		
		write_imagei(new_ids,(int2) (j, gX), convert_int4(itemp[1]));
		write_imagef(new_dis,(int2) (j, gX), convert_float4(temp[1]));
	}

	barrier(CLK_GLOBAL_MEM_FENCE);
}

__kernel void init_bufferi(__global int* buffer, const int value, const uint width, const uint height){
	uint gX = get_global_id(0);
	uint gY = get_global_id(1);
	uint gH = get_global_size(1);
	uint id;
	for(uint i=0;i<height;i+=gH)
		buffer[(i + gY)*width + gX] = value;
}

__kernel void init_bufferf(__global float* buffer, const float value, const uint width, const uint height){
	uint gX = get_global_id(0);
	uint gY = get_global_id(1);
	uint gH = get_global_size(1);
	
	for(uint i=0;i<height;i+=gH)
		buffer[(i + gY)*width + gX] = value;
}

__kernel void init_image2di(__write_only image2d_t image, const int value, const uint width, const uint height){
	uint gX = get_global_id(0);
	uint gY = get_global_id(1);
	uint gH = get_global_size(1);
	
	for(uint i=0;i<height;i+=gH)
		write_imagei(image,(int2) (gX, i + gY), (int4) value);	
}

__kernel void init_image2df(__write_only image2d_t image, const float value, const uint width, const uint height){
	uint gX = get_global_id(0);
	uint gY = get_global_id(1);
	uint gH = get_global_size(1);
	
	for(uint i=0;i<height;i+=gH)
		write_imagef(image,(int2) (gX, i + gY), (float4) value);	
}
