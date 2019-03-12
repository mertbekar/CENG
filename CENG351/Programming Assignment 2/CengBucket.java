import java.util.ArrayList;

public class CengBucket {

	// GUI-Based Methods
	// These methods are required by GUI to work properly.
	
	public int coinCount()
	{
		// TODO: Return the coin count in the bucket.
		return this.cengCoins.size();		
	}
	
	public CengCoin coinAtIndex(int index)
	{
		// TODO: Return the corresponding coin at the index.
		if(index < this.cengCoins.size())
		{
			return this.cengCoins.get(index);
		}
		else
		{
			return null;
		}
	}
	
	public int getHashPrefix()
	{
		// TODO: Return hash prefix length.
		return this.hashDigit;
	}
	
	public Boolean isVisited()
	{
		// TODO: Return whether the bucket is found while searching.
		return this.visited;		
	}
	
	// Own Methods

	private ArrayList<CengCoin> cengCoins;
	private int hashDigit;
	private Boolean visited;

	public CengBucket(int hashDigit)
	{
		this.hashDigit = hashDigit;
		this.cengCoins = new ArrayList<CengCoin>();
		this.visited = false;
	} 

	public void addCoin(CengCoin coin)
	{
		this.cengCoins.add(coin);
	}

	public void incrHashDigit()
	{
		this.hashDigit = this.hashDigit + 1;
	}

	public void removeCoin(int index)
	{
		this.cengCoins.remove(index);
	}

	public void setVisited(Boolean visited)
	{
		this.visited = visited;
	}

	public void print()
	{
		System.out.print("	");
		System.out.println("<bucket>");
		System.out.print("	");
		System.out.print("	");
		System.out.print("<hashLength>");
		System.out.print(this.hashDigit);
		System.out.println("</hashLength>");
		for(int i = 0; i < this.coinCount(); i++)
		{
			this.coinAtIndex(i).print();
		}
		System.out.print("	");
		System.out.println("</bucket>");
	}
}
