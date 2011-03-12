
__kernel void cluster(unsigned int size){
/*	int HALF = SIZE/2 + 1;
	int i = 0; 
	int j = get_global_id(0);

		for(int k=1;k<HALF;k++){
			if( j < SIZE ) {
				i++;
				j++;
				dis(i,j);				
			} else {
				i = HALF;
				j = SIZE - gid + i;
				dis1(i,j);
			} 
		}
		cout << endl;
*/
};

__kernel void distances(__write_only image2d_t result, __global float2* f0, __global float2* f1, const uint size){
	uint globalIdx = get_global_id(0);
	uint globalIdy = get_global_id(1);

	int i = globalIdy;
	
	while(i<size){
		write_imagef(result, (int2) (i,globalIdx), (float4) distance(f0[i], f1[globalIdx]));
		i+=4;
	}
};


__kernel void loop(__global float* f0, __global float* f1){
	
};
