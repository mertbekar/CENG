import java.util.ArrayList;
import java.lang.Math;

public class CengHashTable {

	public CengHashTable()
	{
		// TODO: Create a hash table with only 1 row.
		this.hashRows = new ArrayList<CengHashRow>();
		this.bitCount = 0;
		this.flag = 0;
		this.visitedHashRows = new ArrayList<CengHashRow>();
		this.visitedBuckets = new ArrayList<CengBucket>();
		this.flag2 = 0;
		CengHashRow hashRow = new CengHashRow("0");
		CengBucket bucket = new CengBucket(0);
		CengCoinExchange.getBucketList().addBucket(bucket);
		hashRow.setBucket(bucket);
		hashRows.add(hashRow);
	}

	public void addCoin(CengCoin coin)
	{
		int hValue = (coin.key() % CengCoinExchange.getHashMod());
		int fix = (int) (Math.log(CengCoinExchange.getHashMod()) / Math.log(2));
		int i = 0;
		int ind = 0;
		if(this.hashRows.size() == 0)
		{
			CengHashRow hRow = this.hashRows.get(0);
			CengBucket bucket = hRow.getBucket();
			bucket.addCoin(coin);
		}
		else if(this.hashRows.size() == 1)
		{
			CengHashRow hRow = this.rowAtIndex(0);
			CengBucket bucket = hRow.getBucket();
			if(bucket.coinCount() < CengCoinExchange.getBucketSize())
			{
				bucket.addCoin(coin);
			}
			else
			{
				this.extend(hRow, coin);
			}
		}
		else
		{
			while(i < this.rowCount())
			{
				String ff = Integer.toBinaryString(hValue);
				//System.out.println(ff);
				if(Integer.toBinaryString(hValue).length() > fix)
				{
					ff = Integer.toBinaryString(hValue).substring(0, fix);
				}
				else if(Integer.toBinaryString(hValue).length() < fix)
				{
					ff = Integer.toBinaryString(hValue);
					while(ff.length() < fix)
					{
						ff = "0"+ff;
					}
				}
				ff = ff.substring(0, this.prefixBitCount());
				String fd = hashRows.get(i).hashPrefix();
				if(ff.equals(fd))
				{
					CengHashRow hRow = this.rowAtIndex(i);
					CengBucket bucket = hRow.getBucket();
					ind = CengCoinExchange.getBucketList().getIndex(bucket);
					//System.out.println(hRow.getFlag());
					if(bucket.coinCount() < CengCoinExchange.getBucketSize())
					{
						hashRows.get(i).getBucket().addCoin(coin);
						break;
					}
					else if(hRow.getFlag() == 1)
					{
						hRow.getBucket().incrHashDigit();
						//hRow.setFlag(0);
						/*CengBucket buket = new CengBucket(this.bitCount);
						if(CengCoinExchange.getBucketList().bucketCount() > i)
						{
							CengCoinExchange.getBucketList().addBucket(buket, i);
						}
						else
						{
							CengCoinExchange.getBucketList().addBucket(buket);
						}*/
						for(int u = 0; u < CengCoinExchange.getBucketSize(); u++)
						{
							this.q.add(this.rowAtIndex(i).getBucket().coinAtIndex(0));
							this.rowAtIndex(i).getBucket().removeCoin(0);
						}
						split(i);
						//hRow.setBucket(buket);
						int h = this.q.size();//this.rowAtIndex(i-1).getBucket().coinCount();
						while(h > 0)
						{
							CengCoin cCoin = this.q.get(0);//rowAtIndex(i-1).getBucket().coinAtIndex(0);
							this.addCoin(cCoin);
							this.q.remove(0);
							h--;
						}
						this.addCoin(coin);
						break;
					}
					else if(i+1 < this.rowCount() && hashRows.get(i+1).getFlag() == 1)
					{
						//hashRows.get(i+1).setFlag(0);
						if(bucket.coinCount() < CengCoinExchange.getBucketSize())
						{
							this.addCoin(coin);
							break;
						}
						else
						{
							hRow.getBucket().incrHashDigit();
							/*CengBucket tBucket = new CengBucket(this.bitCount);
							hashRows.get(i+1).setBucket(tBucket);*/
							for(int u = 0; u < CengCoinExchange.getBucketSize(); u++)
							{
								this.q.add(this.rowAtIndex(i).getBucket().coinAtIndex(0));
								this.rowAtIndex(i).getBucket().removeCoin(0);
							}
							split(i);
							//System.out.println(hRow.hashPrefix());
							int h = this.q.size();//rowAtIndex(i).getBucket().coinCount();
							while(h > 0)
							{
								CengCoin cCoin = this.q.get(0);//rowAtIndex(i-1).getBucket().coinAtIndex(0);
								this.addCoin(cCoin);
								this.q.remove(0);
								h--;
							}
							this.addCoin(coin);
							/*if(CengCoinExchange.getBucketList().bucketCount() > i)
							{
								CengCoinExchange.getBucketList().addBucket(tBucket, i+1);
							}
							else
							{
								CengCoinExchange.getBucketList().addBucket(tBucket);
							}*/
							break;
						}
					}	
					else
					{
						this.extend(hRow, coin);
						break;
					}
				}
				i++;
			}
		}
	}
	
	public void searchCoin(Integer key)
	{
		// TODO: Empty Implementation
		int fix = (int) (Math.log(CengCoinExchange.getHashMod()) / Math.log(2));
		System.out.println("BEGIN SEARCH");
		if(this.visitedHashRows.size() > 0)
		{
			while(this.visitedHashRows.size() > 0)
			{
				this.visitedHashRows.get(0).setVisited(false);
				this.visitedHashRows.remove(0);
			}
		}
		if(this.visitedBuckets.size() > 0)
		{
			while(this.visitedBuckets.size() > 0)
			{
				this.visitedBuckets.get(0).setVisited(false);
				this.visitedBuckets.remove(0);
			}
		}
		String ss = Integer.toBinaryString(key);
		if(ss.length() < fix)
		{
			while(ss.length() < fix)
			{
				ss = "0"+ss;
			}
		}
		else if(ss.length() > fix)
		{
			ss = ss.substring(0, fix);
		}
		ss = ss.substring(0, this.bitCount);
		for(int i = 0; i<this.rowCount(); i++)
		{
			CengHashRow hRow = this.rowAtIndex(i);
			//if(ss.equals(hRow.hashPrefix()))
			for(int j = 0; j <hRow.getBucket().coinCount(); j++)
			{
				if(hRow.getBucket().coinAtIndex(j).key() == (int) key)
				{
					hRow.setVisited(true);
					visitedHashRows.add(hRow);
					hRow.getBucket().setVisited(true);
					visitedBuckets.add(hRow.getBucket());
					hRow.print();
					break;
				}
				/*hRow.setVisited(true);
				visitedHashRows.add(hRow);
				hRow.getBucket().setVisited(true);
				visitedBuckets.add(hRow.getBucket());
				if(hRow.getBucket().getHashPrefix() < this.bitCount)
				{
					int k = hRow.getBucket().getHashPrefix();
					k = this.bitCount - k;
					int u = (int) java.lang.Math.pow(2, k);
					for(int j = 0; j < this.rowCount(); j++)
					{
						if(this.rowAtIndex(j).hashPrefix().substring(0, this.rowAtIndex(j).hashPrefix().length() - k).equals(ss.substring(0, ss.length() - k)))
						{
							this.rowAtIndex(j).setVisited(true);
							this.visitedHashRows.add(this.rowAtIndex(j));
						}
					}
				}*/
			}
		}
		if(visitedHashRows.size() == 0)
		{
			System.out.println("None");
		}
		System.out.println("END SEARCH");
	}
	
	public void print()
	{
		// TODO: Empty Implementation
		System.out.println("BEGIN TABLE");
		for(int i = 0; i < this.rowCount(); i++)
		{
			this.rowAtIndex(i).print();
		}
		System.out.println("END TABLE");
	}

	// GUI-Based Methods
	// These methods are required by GUI to work properly.
	
	public int prefixBitCount()
	{
		// TODO: Return table's hash prefix length.
		return this.bitCount;		
	}
	
	public int rowCount()
	{
		// TODO: Return the count of HashRows in table.
		return hashRows.size();		
	}
	
	public CengHashRow rowAtIndex(int index)
	{
		// TODO: Return corresponding hashRow at index.
		return hashRows.get(index);
	}
	
	private ArrayList<CengHashRow> hashRows;
	private int first = 1;
	private int flag = 0;
	private int bitCount;
	private int flag2 = 0;
	private ArrayList<CengHashRow> visitedHashRows;
	private ArrayList<CengBucket> visitedBuckets;
	public ArrayList<CengCoin> q = new ArrayList<CengCoin>();

	// Own Methods

	public void extend(CengHashRow hashRow, CengCoin coin)
	{
		// TODO: Return corresponding hashRow at index.
		//System.out.println("extend");
		int fix = (int) (Math.log(CengCoinExchange.getHashMod()) / Math.log(2));
		int j = this.rowCount();
		if(this.rowCount() == 1)
		{
			CengHashRow hRow = new CengHashRow("1");
			CengBucket bucket = new CengBucket(this.prefixBitCount() + 1);
			hRow.setBucket(bucket);
			hashRows.get(0).getBucket().incrHashDigit();
			CengCoinExchange.getBucketList().addBucket(bucket);
			this.hashRows.add(hRow);
			this.bitCount = this.bitCount + 1;
			int u = hashRows.get(0).getBucket().coinCount();
			while(u>0)
			{
				CengCoin cCoin = this.hashRows.get(0).getBucket().coinAtIndex(0);
				this.hashRows.get(0).getBucket().removeCoin(0);
				this.addCoin(cCoin);
				u--;
			}
			if(flag2 == 0)
			{
				//System.out.println("ekledim");
				//flag2 = 1;
				//this.addCoin(coin);
			}
			this.addCoin(coin);
		}
		else
		{
			this.bitCount = this.bitCount + 1;
			int y = 0;
			int i = (int) java.lang.Math.pow(2, this.prefixBitCount());
			for(int k = 0; k < i; k++)
			{
				CengHashRow hRow2 = this.hashRows.get(k);
				String ss = hRow2.hashPrefix();
				String ss3 = ss;
				String ss2 = ss;
				ss = ss+"0";
				hRow2.changeHashPrefix(ss);
				ss2 = ss2+"1";
				CengHashRow hRow = new CengHashRow(ss2);
				if(hashRow.hashPrefix().substring(0, this.bitCount-1).equals(ss3))
				{
					CengBucket bucket = new CengBucket(this.bitCount);
					CengCoinExchange.getBucketList().addBucket(bucket, k+1-y);
					hRow.setBucket(bucket);
					this.hashRows.add(k+1, hRow);
					hRow2.getBucket().incrHashDigit();
				}
				else
				{
					hRow.setFlag(1);
					//hRow2.setFlag(1);
					hRow.setBucket(hRow2.getBucket());
					this.hashRows.add(k+1, hRow);
					y++;
				}
				k++;
			}
			int h = hashRow.getBucket().coinCount();
			//this.bitCount = this.bitCount - 1;
			while(h > 0)
			{
				CengCoin cCoin = hashRow.getBucket().coinAtIndex(0);
				hashRow.getBucket().removeCoin(0);
				this.addCoin(cCoin);
				h--;
			}
			if(flag2 == 0)
			{
				flag2 = 1;
				//System.out.println("ekleme");
				//this.addCoin(coin);
			}
			this.addCoin(coin);
			//this.bitCount = this.bitCount + 1;
			/*for(int k = 0; k < i; k++)
			{
				int h = this.hashRows.get(k).getBucket().coinCount();
				if(h > 0)
				{
					while(h > 0)
					{
						CengCoin cCoin = this.hashRows.get(k).getBucket().coinAtIndex(0);
						this.hashRows.get(k).getBucket().removeCoin(0);
						this.addCoin(cCoin);
						h--;
					}
				}
			}*/
		}
		//this.addCoin(coin);
		flag2 = 0;
		
	}

	public void incrBitCount()
	{
		this.bitCount = this.bitCount + 1;
	}

	public void split(int index)
	{
		CengHashRow fRow = this.hashRows.get(index);
		CengHashRow tRow = fRow;
		//System.out.println("ok");
		//System.out.println(fRow.hashPrefix());
		while(fRow.getFlag() == 1)
		{
			index = index - 1;
			fRow = this.hashRows.get(index);
		}
		String ss = fRow.hashPrefix();
		//System.out.println(ss);
		ss = ss.substring(0, fRow.getBucket().getHashPrefix());
		while(ss.equals(fRow.hashPrefix().substring(0, fRow.getBucket().getHashPrefix())) && index < this.rowCount() - 1)
		{
			index = index + 1;
			fRow = this.hashRows.get(index);
		}
		String ss2 = fRow.hashPrefix();
		/*ss = ss.substring(0, fRow.getBucket().getHashPrefix());
		while(index < this.rowCount() && ss2.substring(0, fRow.getBucket().getHashPrefix()- 1).equals(fRow.hashPrefix().substring(0, fRow.getBucket().getHashPrefix() - 1)))
		{*/
			ss = fRow.hashPrefix();
			//System.out.println("ok");
			ss = ss.substring(0, fRow.getBucket().getHashPrefix());
			CengBucket bucket = new CengBucket(fRow.getBucket().getHashPrefix());
			if(index < CengCoinExchange.getBucketList().bucketCount())
			{
				CengCoinExchange.getBucketList().addBucket(bucket, index);
			}
			else
			{
				CengCoinExchange.getBucketList().addBucket(bucket);
			}
			int c = index;
			while(fRow.hashPrefix().substring(0, fRow.getBucket().getHashPrefix())/*) && index < this.rowCount() && ss2.substring(0, fRow.getBucket().getHashPrefix()- 1).equals(fRow.hashPrefix().substring(0, fRow.getBucket().getHashPrefix() - 1)))*/.equals(this.rowAtIndex(index).hashPrefix().substring(0, fRow.getBucket().getHashPrefix())))
			{
				this.rowAtIndex(index)./*fRow.*/setBucket(bucket);
				if(this.rowAtIndex(index).hashPrefix().equals(fRow.hashPrefix()))
				{
					this.rowAtIndex(index).setFlag(0);
				}
				else
				{
					this.rowAtIndex(index).setFlag(1);
				}
				index = index + 1;
				if(index >= this.rowCount())
				{
					//fRow = this.hashRows.get(index);
					break;
				}
				/*else
				{
					//index = index - 1;
					break;
				}
			}
			/*if(c+1 == index)
			{
				this.hashRows.get(index-1).setFlag(0);*/
			}
	}
}
