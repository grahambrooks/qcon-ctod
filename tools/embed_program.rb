
puts ARGV
input_path = ARGV[0]
output_path = ARGV[1]

puts "Processing #{input_path} to #{output_path}"

File.open(output_path, "w") do |output|

  output.puts "const char *KernelSource = \"\\n\" \\"

  File.open(input_path) do |input|
    input.each_line do |line|
      output.puts "\"#{line.strip}\\n\" \\"
    end
  end
  output.puts "\"\\n\""
end
