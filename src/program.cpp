const char *KernelSource = "\n" \
"__kernel void square(\n" \
"__global float* input,\n" \
"__global float* output,\n" \
"const unsigned int count)\n" \
"{\n" \
"size_t i = get_global_id(0);\n" \
"if(i < count)\n" \
"output[i] = input[i] * input[i];\n" \
"}\n" \
"\n";
