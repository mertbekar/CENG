import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.List;
import java.io.FileReader;

public class CengCoinParser {

	public static ArrayList<CengCoin> parseCoinsFromFile(String filename)
	{
		ArrayList<CengCoin> coinList = new ArrayList<CengCoin>();
		String[] str;
		File input = new File(filename);
		Integer key;
		String name;
		String value;
		String currency;
		try{
			Scanner inp = new Scanner(input);
			while(inp.hasNextLine())
            {
                String line = inp.nextLine();
                str = line.split("\\|");
                key = Integer.parseInt(str[1]);
                name = str[2];
                value = str[3];
                currency = str[4];
                CengCoin coin = new CengCoin(key, name, value, currency);
                coinList.add(coin);
            }
		}
		catch(FileNotFoundException e){
                e.printStackTrace();
                }
		// You need to parse the input file in order to use GUI tables.
		// TODO: Parse the input file, and convert them into CengCoins

		return coinList;
	}
	
	public static void startParsingCommandLine() throws IOException
	{
		Scanner scanner = new Scanner(System.in);
		String[] str;
		Integer key;
		String command;
		String name;
		String value;
		String currency;
		int i;
		while(true)
		{
			String line = scanner.nextLine();
			str = line.split("\\|");
			command = str[0];
			if(command.equalsIgnoreCase("quit"))
			{
				System.exit(0);
				break;
			}
			if(command.equalsIgnoreCase("add"))
			{
				key = Integer.parseInt(str[1]);
				name = str[2];
				value = str[3];
				currency = str[4];
				CengCoin coin = new CengCoin(key, name, value, currency);
				CengCoinExchange.addCoin(coin);
			}
			if(command.equalsIgnoreCase("search"))
			{
				key = Integer.parseInt(str[1]);
				CengCoinExchange.searchCoin(key);
			}
			if(command.equalsIgnoreCase("print"))
			{
				CengCoinExchange.printEverything();
			}
		}
		// TODO: Start listening and parsing command line -System.in-.
		// There are 4 commands:
		// 1) quit : End the app, gracefully. Print nothing, call nothing.
		// 2) add : Parse and create the coin, and call CengCoinExchange.addCoin(newlyCreatedCoin).
		// 3) search : Parse the key, and call CengCoinExchange.searchCoin(parsedKey).
		// 4) print : Print the whole hash table with the corresponding buckets, call CengCoinExchange.printEverything().

		// Commands (quit, add, search, print) are case-insensitive.
	}
}
