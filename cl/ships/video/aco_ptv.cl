__constant sampler_t imageSampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST; 

__kernel void distances(__write_only image2d_t result, __global const float2* f0, __global  const float2* f1, const uint size){
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
 * @param dis the distance matrix
 * @param clusters output clusters
 * @param cluster in shared memory
 * @param cluster_size size of cluster
 * @param num_particles number of particles
 * 
 */
 
__kernel void clusters(__read_only image2d_t dis, __write_only image2d_t clusters, __local int* cluster, const uint cluster_size, const uint num_particles){
	uint globalIdx = get_global_id(0);
	uint localIdx = get_local_id(0);
	
	float temp1=0.0f;
	float temp[]={0.0f,1000.0f};
	float4 elem;
	int itemp[2];	
	
	for(uint j=0;j<cluster_size;j++){
		temp[1] = 1000.0f;
		for(uint i=0;i<num_particles;++i){
			elem = read_imagef(dis, imageSampler, (int2)(i, globalIdx));
			itemp[(int)((temp[1] > elem.x) && (temp1 < elem.x))] = i;
			temp[(int)((temp[1] > elem.x) && (temp1 < elem.x))] = elem.x;
		}
		temp1 = temp[1];
		cluster[j + localIdx*8] = itemp[1];
	}
	barrier(CLK_LOCAL_MEM_FENCE);

	for(uint i=0;i<cluster_size;i++)
		write_imagei(clusters,(int2) (i, globalIdx), convert_int4(cluster[i + localIdx*8]));

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
