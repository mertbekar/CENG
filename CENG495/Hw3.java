import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.Partitioner;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class Hw3 {

  static int counter = 0;
  static Text longest = new Text("longest");

  public static class MapLength
       extends Mapper<Object, Text, Text, IntWritable>{

    private final static IntWritable one = new IntWritable(1);
    private Text word = new Text();

    public void map(Object key, Text value, Context context
                    ) throws IOException, InterruptedException {
      StringTokenizer itr = new StringTokenizer(value.toString());
      int length;
      IntWritable counter_writable;
      while (itr.hasMoreTokens()) {
        word.set(itr.nextToken());
        length = word.getLength();
        if(counter < length)
        {
        	counter = length;
        	counter_writable = new IntWritable(counter);
        	context.write(longest, counter_writable);
        }
      }
    }
  }

  public static class ReduceLength
       extends Reducer<Text,IntWritable,Text,IntWritable> {
    private IntWritable result = new IntWritable();
    public void reduce(Text key, Iterable<IntWritable> values,
                       Context context
                       ) throws IOException, InterruptedException {
      result.set(counter);
      context.write(longest, result);
    }
  }

    public static class MapLetter
       extends Mapper<Object, Text, Text, IntWritable>{

    private final static IntWritable one = new IntWritable(1);
    private Text word = new Text();
    public void map(Object key, Text value, Context context
                    ) throws IOException, InterruptedException {
      StringTokenizer itr = new StringTokenizer(value.toString());
      while (itr.hasMoreTokens()) {
        word.set(itr.nextToken());
        String str = word.toString();
        int length = str.length();
        Text text;
        for(int i = 0; i < length; i++)
        {
        	String str2 = Character.toString(str.charAt(i));
        	text = new Text(str2);
        	context.write(text, one);
        }
      }
    }
  }

   public static class LetterPartitioner extends
   Partitioner < Text, IntWritable >
   {
   	    public static boolean isVowel(char c)
    	{
  			return "aeiou".indexOf(c) != -1;
		}
      @Override
      public int getPartition(Text key, IntWritable value, int numReduceTasks)
      {
	    String str = key.toString();
	    char c = str.charAt(0);
	    if(isVowel(c))
	    {
			return 0;      
		}
		else
		{
			return 1;    
		}
      }
   }


  public static class ReduceLetter
       extends Reducer<Text,IntWritable,Text,IntWritable> {
    private IntWritable result = new IntWritable();

    public void reduce(Text key, Iterable<IntWritable> values,
                       Context context
                       ) throws IOException, InterruptedException {
    int sum = 0;
    for (IntWritable val : values)
    {
    	sum += val.get();
    }
    result.set(sum);
    context.write(key, result);
  }
}

  public static class MapCount
       extends Mapper<Object, Text, Text, IntWritable>{

    private final static IntWritable one = new IntWritable(1);
    private Text word = new Text();

    public void map(Object key, Text value, Context context
                    ) throws IOException, InterruptedException {
      StringTokenizer itr = new StringTokenizer(value.toString());
      while (itr.hasMoreTokens()) {
        word.set(itr.nextToken());
        context.write(word, one);
      }
    }
  }

  public static class CountPartitioner extends
   Partitioner < Text, IntWritable >
   {
      @Override
      public int getPartition(Text key, IntWritable value, int numReduceTasks)
      {
	    if(key.getLength() < 4)
      	{
    		return 0;
      	}
      	else if(key.getLength() > 3 && key.getLength() < 7)
      	{
    		return 1;
      	}
      	else
      	{
      	   return 2;
      	}
      }
   }

  public static class ReduceCount
       extends Reducer<Text,IntWritable,Text,IntWritable> {
    private IntWritable result = new IntWritable();
    public void reduce(Text key, Iterable<IntWritable> values,
                       Context context
                       ) throws IOException, InterruptedException {
      int sum = 0;
      for (IntWritable val : values) {
        sum += val.get();
      }
      result.set(sum);
      context.write(key, result);
    }
  }


  public static void main(String[] args) throws Exception {
  	String order = args[0];
  	if(order.equals("longest"))
  	{
	    Configuration conf = new Configuration();
	    Job job = Job.getInstance(conf, "word count");
	    job.setJarByClass(Hw3.class);
	    job.setMapperClass(MapLength.class);
	    job.setCombinerClass(ReduceLength.class);
	    job.setReducerClass(ReduceLength.class);
	    job.setOutputKeyClass(Text.class);
	    job.setOutputValueClass(IntWritable.class);
	    FileInputFormat.addInputPath(job, new Path(args[1]));
	    FileOutputFormat.setOutputPath(job, new Path(args[2]));
	    System.exit(job.waitForCompletion(true) ? 0 : 1);  		
  	}
  	else if(order.equals("letter")) {
	    Configuration conf = new Configuration();
	    Job job = Job.getInstance(conf, "word count");
	    job.setJarByClass(Hw3.class);
	    job.setMapperClass(MapLetter.class);
	    job.setCombinerClass(ReduceLetter.class);
	    job.setReducerClass(ReduceLetter.class);
	    job.setOutputKeyClass(Text.class);
	    job.setOutputValueClass(IntWritable.class);
	    FileInputFormat.addInputPath(job, new Path(args[1]));
	    FileOutputFormat.setOutputPath(job, new Path(args[2]));
	    job.setPartitionerClass(LetterPartitioner.class);
	    job.setNumReduceTasks(2);
	    System.exit(job.waitForCompletion(true) ? 0 : 1);  		
  	}
  	else if(order.equals("count"))
  	{
  		Configuration conf = new Configuration();
	    Job job = Job.getInstance(conf, "word count");
	    job.setJarByClass(Hw3.class);
	    job.setMapperClass(MapCount.class);
	    job.setCombinerClass(ReduceCount.class);
	    job.setReducerClass(ReduceCount.class);
	    job.setOutputKeyClass(Text.class);
	    job.setOutputValueClass(IntWritable.class);
	    FileInputFormat.addInputPath(job, new Path(args[1]));
	    FileOutputFormat.setOutputPath(job, new Path(args[2]));
	   	job.setPartitionerClass(CountPartitioner.class);
	    job.setNumReduceTasks(3);
	    System.exit(job.waitForCompletion(true) ? 0 : 1);  	
  	}
  }
}