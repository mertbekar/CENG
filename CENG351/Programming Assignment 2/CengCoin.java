import java.util.ArrayList;

public class CengCoin {
	
	private Integer key;
	
	private String name;
	private String value;
	private String currency;
	
	public CengCoin(Integer key, String name, String value, String currency)
	{
		this.key = key;
		this.name = name;
		this.value = value;
		this.currency = currency;
	}
	
	// Getters
	
	public Integer key()
	{
		return key;
	}
	public String name()
	{
		return name;
	}
	public String value()
	{
		return value;
	}
	public String currency()
	{
		return currency;
	}
		
	// GUI method - do not modify
	public String fullName()
	{
		return "" + key() + "|" + name() + "|" + value() + "|" + currency;
	}
		
	// Own Methods

	public void print()
	{
		int fix = (int) (Math.log(CengCoinExchange.getHashMod()) / Math.log(2));
		System.out.print("	");
		System.out.print("	");
		System.out.println("<coin>");
		System.out.print("	");
		System.out.print("	");
		System.out.print("	");
		System.out.print("<hash>");
		int hValue = (this.key() % CengCoinExchange.getHashMod());
		String ss = Integer.toBinaryString(hValue);
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
		System.out.print(ss);
		System.out.println("</hash>");
		System.out.print("	");
		System.out.print("	");
		System.out.print("	");
		System.out.print("<value>");
		System.out.print(this.key());
		System.out.print("|");
		System.out.print(this.name());
		System.out.print("|");
		System.out.print(this.value());
		System.out.print("|");
		System.out.print(this.currency());
		System.out.println("</value>");
		System.out.print("	");
		System.out.print("	");
		System.out.println("</coin>");
	}
}
