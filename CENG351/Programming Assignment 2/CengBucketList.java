import java.util.ArrayList;

public class CengBucketList {

	public void addCoin(CengCoin coin)
	{
		// TODO: Empty Implementation
	}
	
	public void searchCoin(Integer key)
	{
		// TODO: Empty Implementation
	}
	
	public void print()
	{
		// TODO: Empty Implementation
	}
	
	// GUI-Based Methods
	// These methods are required by GUI to work properly.
	
	public int bucketCount()
	{
		// TODO: Return all bucket count.
		return cengBuckets.size();		
	}
	
	public CengBucket bucketAtIndex(int index)
	{
		// TODO: Return corresponding bucket at index.
		if(index < cengBuckets.size())
		{
			return cengBuckets.get(index);
		}
		else
		{
			return null;
		}
	}
	
	// Own Methods
	private ArrayList<CengBucket> cengBuckets;

	public CengBucketList()
	{
		//CengBucket bucket = new CengBucket();
		this.cengBuckets = new ArrayList<CengBucket>();
		//this.cengBuckets.add(bucket);
	}

	public void extendBuckets()
	{
		return;
	}

	public void addBucket(CengBucket bucket)
	{
		this.cengBuckets.add(bucket);
	}

	public void addBucket(CengBucket bucket, int index)
	{
		if(index < this.bucketCount())
		{
			this.cengBuckets.add(index, bucket);
		}
		else
		{
			this.addBucket(bucket);
		}
	}

	public int getIndex(CengBucket bucket)
	{
		for(int i = 0; i < this.bucketCount(); i++)
		{
			if(bucket == cengBuckets.get(i))
			{
				return i;
			}
		}
		return 0;
	}
}
