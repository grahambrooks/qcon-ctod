
require 'digest/sha1'

buckets = {}

# Accumulates n lines of data and then adds a bucket for the resulting hash
class Accumulator
  def initialize(length, buckets)
    @length = length
    @buckets = buckets
    @accumulated = ""
    @count = 1
    @context = nil
  end
  
  def add(s, context)
    if @count.modulo(@length) == 0
      emit
    end
    @accumulated += s
    @context = context if @context.nil?
    @count += 1
  end
  
  def emit
#    puts "Emiting for #{@accumulated}"
    sha = Digest::SHA1.hexdigest @accumulated
    @context[:lines] = @accumulated
    
    if @buckets[sha]
      @buckets[sha] << @context
    else
      @buckets[sha] = [@context]
    end
    @accumulated = ""
    @context = nil
  end
end

size = 20

accumulators = []

(0..size).each do
  accumulators << Accumulator.new(size, buckets)
end


if ARGV.length != 2
  print <<EOD
Usage spike project-path patter
EOD
  exit(1)
end

Dir.glob File.join(File.expand_path(ARGV[0]), ARGV[1]) do |path|
  puts "Scanning #{path}"
  
  File.open(path) do |f|
    line_number = 1
    f.each_line do |line|
#      puts "#{line_number.modulo(size)}Processing line #{line}"
      (0..line_number.modulo(size)).each do |index|
        accumulators[index].add(line, { :file => path, :line_number => line_number, :line => line })
      end
      line_number += 1
    end
  end
end

buckets.each do |sha, bucket|
  if bucket.length > 1
    puts "Duplicate block"
    lines = bucket[0][:lines]
    print lines
    puts "In files:"
    bucket.each do |dup|
      puts "#{dup[:line_number]} => #{dup[:file]}"
      if lines != dup[:lines]
        puts "Duplication failure #{lines}"
        puts "--------------------------"
        puts " #{dup[:lines]}"
      end
    end
    puts
  end
end
