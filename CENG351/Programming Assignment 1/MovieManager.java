
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.List;
import java.sql.Statement;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.sql.ResultSet;

/**
 *
 * @author gorkem
 */
public class MovieManager implements IMovieManager {

    /* These values are read from databaseConfiguration.txt file. */
    private String _USERNAME;
    private String _PASSWORD;
    private String _HOST;
    private String _DATABASE_SCHEMA;
    private int _PORT_NUMBER;

    private static String _DB_CONFIG_FILE_NAME = "databaseConfiguration.txt";
    private static String _ACTOR_FILE_NAME = "actors.txt";
    private static String _MOVIE_FILE_NAME = "movies.txt";
    private static String _DIRECTOR_FILE = "directors.txt";
    private static String _USER_FILE = "users.txt";

    private Connection _connection;

    // DO NOT MODIFY THIS FUNCTION
    public void ReadDatabaseConfiguration() throws Exception {
        BufferedReader br = null;

        try {
            String currentLine;
            br = new BufferedReader(new FileReader(this._DB_CONFIG_FILE_NAME));

            while ((currentLine = br.readLine()) != null) {
                if (currentLine.startsWith("host")) {
                    this._HOST = currentLine.replace("host=", "").trim();
                }
                if (currentLine.startsWith("schema")) {
                    this._DATABASE_SCHEMA = currentLine.replace("schema=", "").trim();
                }
                if (currentLine.startsWith("username")) {
                    this._USERNAME = currentLine.replace("username=", "").trim();
                }
                if (currentLine.startsWith("password")) {
                    this._PASSWORD = currentLine.replace("password=", "").trim();
                }
                if (currentLine.startsWith("port")) {
                    this._PORT_NUMBER = Integer.parseInt(currentLine.replace("port=", "").trim());
                }
            }
        } catch (Exception ex) {
            System.out.println("Error while reading databaseConfiguration.txt file: " + ex.getMessage());
            throw ex;
        } finally {
            try {
                if (br != null) {
                    br.close();
                }
            } catch (IOException ex) {
                System.out.println("Error while reading databaseConfiguration.txt file: " + ex.getMessage());
                throw ex;
            }
        }

        System.out.println("Database configuration is read.");
    }

    // DO NOT MODIFY THIS FUNCTION
    public void InitializeConnection() {

        String url = "jdbc:mysql://" + this._HOST + ":" + this._PORT_NUMBER + "/"
                + this._DATABASE_SCHEMA + "?useUnicode=true&characterEncoding=UTF-8";

        try {
            Class.forName("com.mysql.jdbc.Driver");
            this._connection = DriverManager.getConnection(url, this._USERNAME,
                    this._PASSWORD);
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {

            e.printStackTrace();
        }

        System.out.println("Database connection initialized.");
    }

    @Override
    public void ParseFilesAndInsertData() {
        File input = new File("actor.txt");
        String[] str = null;
        int id;
        try {
            Scanner inp = new Scanner(input);
            while(inp.hasNextLine())
            {
                String line = inp.nextLine();
                str = line.split(";");
                id = Integer.parseInt(str[0]);
                Actor actor = new Actor(id, str[1]);
                InsertActor(actor);
            }
        }
        catch(FileNotFoundException e){
                e.printStackTrace();
                }
        
        input = new File("directors.txt");
        str = null;
        try {
            Scanner inp = new Scanner(input);
            while(inp.hasNextLine())
            {
                String line = inp.nextLine();
                str = line.split(";");
                id = Integer.parseInt(str[0]);
                Director director = new Director(id, str[1]);
                InsertDirector(director);
            }
        }
        catch(FileNotFoundException e){
                e.printStackTrace();
                }
        
        input = new File("movies.txt");
        str = null;
        int year;
        float rating;
        Statement ss = null;
        ResultSet result;
        try {
            Scanner inp = new Scanner(input);
            while(inp.hasNextLine())
            {
                String line = inp.nextLine();
                str = line.split(";");
                id = Integer.parseInt(str[0]);
                year = Integer.parseInt(str[2]);
                rating = Float.parseFloat(str[6]);
                try{
                    ss = _connection.createStatement();
                    result = ss.executeQuery("SELECT d.did FROM director d WHERE d.name = str[5];");
                    while(result.next())
                    {
                        id = result.getInt("did");
                    }
                }
                catch(SQLException e){
                    e.printStackTrace();
                }
                catch(Exception ee) {
                    ee.printStackTrace();
                }
                Director director = new Director(id, str[5]);
                Movie movie = new Movie(id, str[1], str[3], rating, year, director);
                InsertMovie(movie);
            }
        }
        catch(FileNotFoundException e){
                e.printStackTrace();
                }
        
        input = new File("users.txt");
        str = null;
        int age;
        try {
            Scanner inp = new Scanner(input);
            while(inp.hasNextLine())
            {
                String line = inp.nextLine();
                str = line.split(";");
                id = Integer.parseInt(str[0]);
                age = Integer.parseInt(str[2]);
                User user = new User(id, str[1], age);
                InsertUser(user);
            }
        }
        catch(FileNotFoundException e){
                e.printStackTrace();
                }
    }

    @Override
    public void CreateTables() {
        Statement ss = null;
        try{
            ss = _connection.createStatement();
            ss.executeUpdate("CREATE TABLE actor" + "(aid INT, name VARCHAR(50), PRIMARY KEY(aid))");
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
        catch(Exception ee)
        {
            ee.printStackTrace();
        }
        try{
            ss = _connection.createStatement();
            ss.executeUpdate("CREATE TABLE director" + "(did INT, name VARCHAR(50), PRIMARY KEY(did))");
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
        catch(Exception ee)
        {
            ee.printStackTrace();
        }
        try{
            ss = _connection.createStatement();
            ss.executeUpdate("CREATE TABLE movie" + "(mid INT, title VARCHAR(100), genre VARCHAR(50), year INT, did INT, rating FLOAT, PRIMARY KEY(mid), FOREIGN KEY(did) REFERENCES(director)");
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
        catch(Exception ee)
        {
            ee.printStackTrace();
        }
        try{
            ss = _connection.createStatement();
            ss.executeUpdate("CREATE TABLE user" + "(uid INT, name VARCHAR(50), age INT, PRIMARY KEY(uid))");
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
        catch(Exception ee)
        {
            ee.printStackTrace();
        }
        try{
            ss = _connection.createStatement();
            ss.executeUpdate("CREATE TABLE casted_in" + "(aid INT, mid INT, PRIMARY KEY(aid, mid), FOREIGN KEY(aid) REFERENCES(actor), FOREING KEY(mid) REFERENCES(movie))");
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
        catch(Exception ee)
        {
            ee.printStackTrace();
        }
        try{
            ss = _connection.createStatement();
            ss.executeUpdate("CREATE TABLE watched" + "(uid INT, mid INT, PRIMARY KEY(uid, mid), FOREIGN KEY(uid) REFERENCES(user), FOREING KEY(mid) REFERENCES(movie))");
            
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
        catch(Exception ee)
        {
            ee.printStackTrace();
        }
    }

    @Override
    public void InsertActor(Actor actor) {
        Statement ss = null;
        try{
            ss = _connection.createStatement();
            ss.executeUpdate("INSERT INTO actor VALUES(" + actor.getAid() + "," + actor.getName() + ")");
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
        catch(Exception ee)
        {
            ee.printStackTrace();
        }
    }

    @Override
    public void InsertDirector(Director director) {
        Statement ss = null;
        try{
            ss = _connection.createStatement();
            ss.executeUpdate("INSERT INTO director VALUES(" + director.getDid() + "," + director.getName() + ")");
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
        catch(Exception ee)
        {
            ee.printStackTrace();
        }
    }


    @Override
    public void InsertMovie(Movie movie) {
        Statement ss = null;
        try{
            ss = _connection.createStatement();
            ss.executeUpdate("INSERT INTO movie VALUES(" + movie.getMid() + "," + movie.getTitle() + "," + movie.getGenre() + "," + movie.getYear() + "," + movie.getDirector().getDid() + "," + movie.getRating() + ")");
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
        catch(Exception ee)
        {
            ee.printStackTrace();
        }

    }

    @Override
    public void InsertUser(User user) {
        Statement ss = null;
        try{
            ss = _connection.createStatement();
            ss.executeUpdate("INSERT INTO user VALUES(" + user.getUid() + "," + user.getName() + "," + user.getAge() + "," +")");
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
        catch(Exception ee)
        {
            ee.printStackTrace();
        }

    }

    @Override
    public void InsertCastedIn(Actor actor, Movie movie) {
        Statement ss = null;
        try{
            ss = _connection.createStatement();
            ss.executeUpdate("INSERT INTO casted_in VALUES(" + actor.getAid() + "," + movie.getMid() + ")");
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
        catch(Exception ee)
        {
            ee.printStackTrace();
        }

    }

    @Override
    public void InsertWatched(User user, Movie movie) {
        Statement ss = null;
        try{
            ss = _connection.createStatement();
            ss.executeUpdate("INSERT INTO watched VALUES(" + user.getUid() + "," + movie.getMid() + ")");
        }
        catch(SQLException e)
        {
            e.printStackTrace();
        }
        catch(Exception ee)
        {
            ee.printStackTrace();
        }

    }

    @Override
    public List<Movie> GetMoviesOfDirector(int did) {
        return null;
    }

    @Override
    public List<Movie> GetMoviesOfActor(int aid) {
        return null;
    }

    @Override
    public List<Movie> GetUserWatchlist(int uid) {
        return null;
    }

    @Override
    public List<Movie> GetMoviesWithTwoActors(String actorName1, String actorName2) {
        return null;
    }

    @Override
    public List<Movie> GetMoviesAboveRating(float rating) {
        return null;
    }

    @Override
    public void ChangeRatingsOfMoviesLike(String pattern, float value) {

    }

    @Override
    public void DeleteDirectorWithId(int did) {

    }

    @Override
    public void DeleteUserWithId(int uid) {

    }

    @Override
    public void PrintViewStatsByMovie() {

    }

    @Override
    public void PrintViewStatsByGenre() {

    }

    @Override
    public void PrintAverageMovieRatingOfDirectors() {

    }

    @Override
    public void DropTables() {

    }

    /* 
     - This main function is for helping you
     - You don't have to send your main function when you submit your code.
        
     - Use this template to continue coding. 
     - Just implement a function above, and call it with example parameters
     in main function below.
     */
    public static void main(String[] args) throws Exception {

        /* ***** DO NOT CHANGE THIS PART FOR YOUR OWN FAVOUR - BEGIN ***** */
		
        // Create new movie manager object
        MovieManager manager = new MovieManager();

        // Read databaseConfiguration.txt file for database settings
        manager.ReadDatabaseConfiguration();

        // Initialize connection with database and
        // Create connection object
        manager.InitializeConnection();

        /* ***** DO NOT CHANGE THIS PART FOR YOUR OWN FAVOUR - END ***** */

        // Rest of the functions can be called here for testing
        
    }
}

