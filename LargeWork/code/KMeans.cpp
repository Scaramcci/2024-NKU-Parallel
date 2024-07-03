import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.util.Tool;

import utils.GlobalInfo;
import utils.MapOutputValueDataType;
import utils.TheException;
import utils.Tools;



public class KMeans extends Configured implements Tool {
	public static int iterationCount = 0;	// 当前是第几次迭代
	
	public static void main(String[] args) {
		
	}
	
	public int run(String[] args) {
		return 0;
	}
	

	public static class TheMapper extends Mapper<LongWritable, Text, IntWritable, MapOutputValueDataType>
	{
		/* 全局文件形如
		 * iteration number 
	       cluster id, cluster coordinates,  # of records assigned to the cluster
	       cluster id, cluster coordinates,  # of records assigned to the cluster
	       cluster id, cluster coordinates,  # of records assigned to the cluster
	       .... .....
		 */
		private String filename;				// 全局文件的文件名
		private ArrayList<GlobalInfo>	list;	// 全局文件中各个cluster的相关信息

		private HashMap<Integer, ArrayList<Integer>> map;
		
		@Override
		public void setup(Context context) 
		{
			// 读取全局文件，初始化iterationNum和list
			
			map = new HashMap<Integer, ArrayList<Integer>>();
		}
		
		@Override
		public void map(LongWritable offset, Text text, Context context) 
			throws TheException, InterruptedException, IOException
		{
			// 读取本节点上的所有待聚类的数据点，每个数据点以空格分开
			StringTokenizer tokenizer = new StringTokenizer(text.toString());
			
			while (tokenizer.hasMoreTokens()){
				// data是待聚类的数据
				int data = Integer.parseInt(tokenizer.nextToken());
				
				// 计算该数据点应该属于哪一个cluster
				Integer cluster_id = new Integer(Tools.cluster(data, list));
				
				// 为每一个数据点输出 <cluster id, coordinates of the cluster>
				if (map.containsKey(cluster_id)) 
					map.get(cluster_id).add(new Integer(data));
				else {
					ArrayList<Integer> arrayList = new ArrayList<Integer>();
					arrayList.add(new Integer(data));
					map.put(cluster_id, arrayList);
				}
			}
		}
		
		/* 将map中的每一个cluster相关的数据发射出去，发射的形式为
		 * key - cluster id
		 * value - 【# of records， average value of the coordinates】
		 */
		@Override
		public void cleanup(Context context) 
			throws InterruptedException, IOException
		{
			for (Integer clusterId : map.keySet()) {
				ArrayList<Integer> arrayList = map.get(clusterId);
				
				int numOfRecords = arrayList.size();
				int sum = 0;
				for (Integer v : arrayList) 
					sum += v;
				
				double average = (double)sum/numOfRecords;
				
				/*
				 * 为每一个cluster id 发射数据
				 */
				context.write(new IntWritable(clusterId), 
						new MapOutputValueDataType(numOfRecords, average));
			}
		}
	}
	
	/*
	 * Reducer的输入数据格式为：
	 * key - clusterId
	 * value - list of MapOutputValueDataType
	 * MapOutputValueDataType 包含 【numOfRecords，average value fo this records】
	 * 		
	 * 输出的数据用于更新全局文件，形如
		iteration number 
   		cluster id TAB cluster coordinates TAB # of records assigned to the cluster
   		cluster id TAB cluster coordinates TAB # of records assigned to the cluster
   		cluster id TAB cluster coordinates TAB # of records assigned to the cluster
	    .... .....
	 */
	public static class TheReducer 
		extends Reducer<IntWritable, MapOutputValueDataType, IntWritable, IntWritable>
	{
		/*
		 * 假定只有1个reducer，否则代码还需要修改
		 */
		
		String outputFileName = "part-r-";	// 输出的文件名
		
		@Override
		public void setup(Context context)
		{
			// 设置输出文件名
			
			// 首先输出iteration number 
		}
		
		@Override
		public void reduce(IntWritable clusterId, Iterable<MapOutputValueDataType> it, Context context)
		{
			String value = "";
			
			// 把同一个cluster id对应的所有records相关信息整合起来（求带权平均值）
			for (MapOutputValueDataType info : it) {
				
			}
		}
	}
	
}
