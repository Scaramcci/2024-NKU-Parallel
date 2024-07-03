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
	public static int iterationCount = 0;	// ��ǰ�ǵڼ��ε���
	
	public static void main(String[] args) {
		
	}
	
	public int run(String[] args) {
		return 0;
	}
	

	public static class TheMapper extends Mapper<LongWritable, Text, IntWritable, MapOutputValueDataType>
	{
		/* ȫ���ļ�����
		 * iteration number 
	       cluster id, cluster coordinates,  # of records assigned to the cluster
	       cluster id, cluster coordinates,  # of records assigned to the cluster
	       cluster id, cluster coordinates,  # of records assigned to the cluster
	       .... .....
		 */
		private String filename;				// ȫ���ļ����ļ���
		private ArrayList<GlobalInfo>	list;	// ȫ���ļ��и���cluster�������Ϣ

		private HashMap<Integer, ArrayList<Integer>> map;
		
		@Override
		public void setup(Context context) 
		{
			// ��ȡȫ���ļ�����ʼ��iterationNum��list
			
			map = new HashMap<Integer, ArrayList<Integer>>();
		}
		
		@Override
		public void map(LongWritable offset, Text text, Context context) 
			throws TheException, InterruptedException, IOException
		{
			// ��ȡ���ڵ��ϵ����д���������ݵ㣬ÿ�����ݵ��Կո�ֿ�
			StringTokenizer tokenizer = new StringTokenizer(text.toString());
			
			while (tokenizer.hasMoreTokens()){
				// data�Ǵ����������
				int data = Integer.parseInt(tokenizer.nextToken());
				
				// ��������ݵ�Ӧ��������һ��cluster
				Integer cluster_id = new Integer(Tools.cluster(data, list));
				
				// Ϊÿһ�����ݵ���� <cluster id, coordinates of the cluster>
				if (map.containsKey(cluster_id)) 
					map.get(cluster_id).add(new Integer(data));
				else {
					ArrayList<Integer> arrayList = new ArrayList<Integer>();
					arrayList.add(new Integer(data));
					map.put(cluster_id, arrayList);
				}
			}
		}
		
		/* ��map�е�ÿһ��cluster��ص����ݷ����ȥ���������ʽΪ
		 * key - cluster id
		 * value - ��# of records�� average value of the coordinates��
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
				 * Ϊÿһ��cluster id ��������
				 */
				context.write(new IntWritable(clusterId), 
						new MapOutputValueDataType(numOfRecords, average));
			}
		}
	}
	
	/*
	 * Reducer���������ݸ�ʽΪ��
	 * key - clusterId
	 * value - list of MapOutputValueDataType
	 * MapOutputValueDataType ���� ��numOfRecords��average value fo this records��
	 * 		
	 * ������������ڸ���ȫ���ļ�������
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
		 * �ٶ�ֻ��1��reducer��������뻹��Ҫ�޸�
		 */
		
		String outputFileName = "part-r-";	// ������ļ���
		
		@Override
		public void setup(Context context)
		{
			// ��������ļ���
			
			// �������iteration number 
		}
		
		@Override
		public void reduce(IntWritable clusterId, Iterable<MapOutputValueDataType> it, Context context)
		{
			String value = "";
			
			// ��ͬһ��cluster id��Ӧ������records�����Ϣ�������������Ȩƽ��ֵ��
			for (MapOutputValueDataType info : it) {
				
			}
		}
	}
	
}
