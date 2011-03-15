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
