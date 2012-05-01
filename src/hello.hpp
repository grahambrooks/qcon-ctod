void initialise_input_data(float* data, size_t element_count) {
  for(auto i = 0; i < element_count; i++)
    data[i] = rand() / (float)RAND_MAX;
}


size_t check_results(float *input_data, float* results, size_t element_count) {
  auto correct = 0;

  for(auto i = 0; i < element_count; i++) {
    if(results[i] == input_data[i] * input_data[i])
      correct++;
  }

  return correct;
}
