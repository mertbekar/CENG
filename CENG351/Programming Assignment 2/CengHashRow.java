public class CengHashRow {

	// GUI-Based Methods
	// These methods are required by GUI to work properly.

	public String hashPrefix()
	{
		// TODO: Return row's hash prefix (such as 0, 01, 010, ...)
		return this.hashPrefix;		
	}
	
	public CengBucket getBucket()
	{
		// TODO: Return the bucket that the row points at.
		return this.bucket;		
	}
	
	public boolean isVisited()
	{
		// TODO: Return whether the row is used while searching.
		return this.visited;		
	}
	
	// Own Methods

	//private int prefix;
	private CengBucket bucket;
	private String hashPrefix;
	private int flag;
	private boolean visited;

	public CengHashRow(String hashPrefix)
	{
		this.hashPrefix = hashPrefix;
		this.bucket = null;
		this.flag = 0;
		this.visited = false;
	}

	public void setBucket(CengBucket bucket)
	{
		this.bucket = bucket;
	}

	public void changeHashPrefix(String hashPrefix)
	{
		this.hashPrefix = hashPrefix;
	}

	public void setFlag(int flag)
	{
		this.flag = flag;
	}

	public int getFlag()
	{
		return this.flag;
	}

	public void setVisited(boolean visited)
	{
		this.visited = visited;
	}

	public boolean getVisited()
	{
		return this.visited;
	}

	public void print()
	{
		System.out.println("<row>");
		System.out.print("	");
		System.out.print("<hashPrefix>");
		System.out.print(this.hashPrefix);
		System.out.println("</hashPrefix>");
		this.getBucket().print();
		System.out.println("</row>");

	}
}
