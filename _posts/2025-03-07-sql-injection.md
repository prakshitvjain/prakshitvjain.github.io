---
title: SQL Injection
date: 2025-03-07 00:00:00 +0000
author: Prakshit Jain
---
> **Note**: This blog is completely based on OWASP guide for prevening SQL Injection Attacks, but it is put in my own words so it can be understood in a simpler way.

SQL injection is a process of injecting or inserting a SQL query as a user input from client to application, that may lead to obtaining sensitive data from the database, modify database, shutdown the DBMS and also issue commands to OS.

#####  Threat Modeling

* It allows attackers to spoof their identity, tamper with data and become administrator of database server.
* SQL injection is common in PHP and ASP applications rather than j2EE and ASP.NET applications due to their respective programmatic interfaces.
* It is a high impact severity.

##### SQL injection attack occurs when:

1. An unintended data enters a program from an untrusted source.
2. The data is used to dynamically construct a SQL query.

# Anatomy of A Typical SQL Injection Vulnerability

### **Input Validation:**

A common SQL injection vulnerability in Java is un-validated input from the user that is taken and appended to the query, and the input is executed as a part of the query. An attacker can manipulate the query in accordance to his will.

Take a look at the below example:
```java
String query = "SELECT balance FROM account WHERE user_name = '" 
+ request.getParameter("customerName")"'":
```

If an attacker passes the username as 
```MySQL
admin OR 1=1
```
Or
```MySQL
admin; DROP TABLE account
```

The query will look something like this
```MySQL
SELECT balance FROM account WHERE user_name = 'admin' OR 1=1;

-- OR

SELECT balance FROM account WHERE user_name = 'admin'; DROP TABLE account;
```
Such attacks are very dangerous and compromise the security of Organization

So, what are the solutions to prevent these?
## Primary Defenses

	1: Use of Prepared Statements (with Parameterized Queries)
	2: Use of Properly Constructed Stored Procedures
	3: Allow-list Input Validation
	4: STRONGLY DISCOURAGED: Escaping All User Supplied Input

### **1. Parameterized queries (Prepared Statements)**

The correct way to prevent SQL injection is to never concatenate user input directly into the query instead use parameterized queries (Prepared Statements). This ensures that the user input is treated as data and not the part of SQL code.

```java
String query = "SELECT balance FROM account WHERE user_name = ?";
PreparedStatement stmt = connection.prepareStatement(query);
stmt.setString(1,request.getParameter("customerName"));
ResultSet result = stmt.executeQuery();
```

This way the attacker will never be able to inject malicious SQL queries as input. Almost all the languages support parameterized queries. Even SQL abstraction layers like HQL (Hibernate Query Language) faces similar type of injection problems called HQL injection.
Using prepared statements also makes your application relatively database independent.

### **2. Stored Procedures**

Even though Stored procedures are not always safe from SQL injections, developers can use certain standard stored procedures constructs that are generally safe if implemented accordingly.
##### Are they Safe?

To ensure security, the developers are required to create SQL statements with parameters that are automatically parameterized. The basic difference between prepared statements and stored procedures is that the SQL code for a stored procedure is stored in the database itself, and is called by the application. Hence making parameterized queries and stored procedures equally effective in safeguarding the application from SQL injection.

##### When use of Stored Procedures are a risk?

Stored Procedures can be a risk when a system is attacked. Consider an example where an application uses a MS SQL Server, with default roles like `db_datareader` , `db_datawriter` and `db_owner`. Before stored procedures came into use, DBAs (Database Administrators) would assign roles to users, depending on the requirements.

The default roles like  `db_datareader` , `db_datawriter` do not have rights to execute stored procedures, and in organizations where user management is centralized with just these 3 default roles, assigning `db_owner` to users would to a potential risk. If an attacker gains access to any of the users or a server, the attacker will have full rights to the database, where previously they had only read access.

##### A Secure Stored Procedure Example in Java
```java
// This input should be validated in terms of length and allowed characters using regular expressions
String custname = request.getParameter("customerName");
try {
	callableStatement cs = connection.prepareCall("{call sp_getBalance(?)}");
	cs.setString(1,custname);
	ResultSet rs = cs.executeQuery();
	//manipulating recieved data
}
catch (SQLexception){
	//logging and error messsage display
}
```

This example uses Java's stored procedure implementation using `callableStatement` to execute database queries. Where `sp_getBalance` is a stored procedure predefined in the database. The `call` in the `{call sp_getBalance(?)` is used to call the stored procedure and the `?` represents the parameter placeholder.

### **3. Allow-List Input Validation**

When faced with SQL queries that can't use bind variables (parameterized queries) such as names of tables or columns and the sort order indicators (ASC or DESC) as well, input validation is the best defense. When names of tables and columns are needed, they should come from the code and not the user parameters.

##### A Secure Sample Example For Allow-List Input Validation

If user parameter values are used to target different tables and columns, then it is merely a symptom of a poor design and a full-redesigning should be considered. Otherwise the developers should map the parameter values to legal/valid table and column names, preventing the user input from ending up in the query.

Take a look at the Dynamic SQL generation
```java
String tableName;
switch(parameter){
	case "valueA":
		tableName = "ordersTable";
		break;
	case "valueB":
		tableName = "shippedTable";
		break;
	default:
		throw new InpuExceptionError("Invalid input for table name");
}
```

In the above example the `tableName` is declared and using `switch`, the user input i.e `parameter` is validated and a value is assigned to `tablename` depending upon different cases. If input is `valueA` then the value of `tableName` will be `"ordersTable"`. The `default` block handles the errors and exceptions effectively. Here the query is dynamically generated based on the user input but it is secure as it is validated prior to acceptance.

##### When to use this Allow-List Input Validation ?

When we say stored procedures are safe, It is assumed that the stored procedures do not include dynamic SQL code generation. It is usually avoided by the developers. But in some scenarios, if there is a need for dynamic SQL code generation, the Allow-list input method or proper escaping must be used to secure the program.

##### A safer way to implement Dynamic Query Generation

For something simple like sort order, it would be a better way to first convert the user supplied data to boolean and then use the boolean value to determine the legal value, that is appended to the query.

For example
```java
public String someMethod (boolean parameter) {
	String query = " SELECT * FROM employees ORDER BY salary " + (parameter ? "ASC" : "DESC");
	return query;
}
```

Like the above example, if possible it is always safer to convert any user input to a non-string format like integer, boolean, time, date, etc. before it is appended to query or used to select a value to append to a query.

Input validation is recommended as a secondary defense in all cases even when using prepared statements or stored procedures to ensure maximum security.

### **4. Escaping All User-Supplied Input**

In this approach, the developer will have to escape all the input supplied by the user before appending it to a query. Escaping input refers to ensuring that any special characters in user-supplied input are converted or escaped into a safe form before they are included in an SQL query. This defense method is highly discouraged as it does not guarantee safety due to various limitations. 

##### How does Escaping work?

For each special character that might interfere with the query, you replace or escape it with a safe equivalent. Different Databases have different ways of handling escaping. 

##### Below are few common SQL special characters and their functions
- Single Quote (`'`): Used to delimit strings in SQL.
- Double Quote (`"`): Often used for quoting column or table names.
- Backslash (`\`): Used as an escape character in many SQL implementations.
- Semicolon (`;`): Indicates the end of a SQL statement.
- Comment (`--` or `/* ... */`): Allows attackers to comment out the remainder of a query.

###### Escaping single quotes (`'`):

Take a look at below query
```Java
String query = 'SELECT * FROM accounts WHERE username = ' + request.getParameter("custName");
ResultSet rs = stmt.executeQuery(query);
```

If the user supplied input is `O'Rielly`, then the query will break and result in error.

To avoid broken queries, the developer must escape the (`'`) manually. This can be done using doubling the single quotes (`''`).

##### A solution to escaping only single quotes (`'`) in Java

```java
String UserInput = request.getParameter("custName");
//Input = O'Rielly
String EscapedInput = UserInput.replace("'","''");
//Now manually construct the query
String query = "SELECT * FROM accounts WHERE username = '" + EscapedInput + "'");
ResultSet rs = stmt.executeQuery(query);
```

This example will not return errors, when provided with input `O'Rielly`. Similarly for different special characters, different escaping methods are to be used. But such practices are highly discouraged as escaping all the user input is very unsafe. Even though the developers cover all edge cases, there is still a huge risk of SQL injection attacks. Better Alternatives like prepared statements are highly recommended.

## Additional Defenses

In Addition to implementing any one of the four primary defenses, these additional defenses can also be implemented for maximizing the security of the application. The additional defenses are:
- **Least Privilege**
- **Allow-List Input Validation**

### Least Privilege

To minimize the damage of a successful SQL injection attack, consider minimizing the privileges assigned to the every database account in your environment i.e The principle of least privilege (PoLP). Use Bottom-Up approach, always start from what access the application accounts require rather than focusing what access you need to take away. 

Accounts that require to only read the data should be given only read privileges and follow the same for write, provide only if absolutely necessary. DO NOT grant DBA or ADMIN privileges to any of the application accounts. Even though it is easy to do so and everything just functions normally but it is extremely dangerous.

##### What is an Application Account?

Application Account refers to a database user account that acts as a bridge between the web application and the database. It is not a person or a software It is just the database account that the web application software uses to store or retrieve data from the database. Any Application account should not contain Admin privileges like `DROP`,`DELETE` or `ALTER`.

#### Minimizing Application and OS privileges

##### Application privilege

In addition to SQL injection Attacks, there exist other threats to your database data, such as attackers manipulating the parameters from a legal value that is authorized for their current account to a value that is unauthorized for them (only meant to be accessible to `root` or the the `admin`).  

For Example
Consider an application where user-supplied input is appended to query the database directly. Even if the user-input is validated using `javaScript`, the attacker could still modify the URL like this:
```URL
https://flipkart.com/product?id=322
```
To 
```URL
https://flipkart.com/product?id=999        //To be accessed only by admin
```

Since the user-input validation is on the client-side, the attacker can easily bypass the authentication. This is why the input-validation should always be done on the server side where attacker has no control. Even if an attacker gains access to a Application account, to minimize the damage, the privileges assigned to the account should be minimal.

##### OS privilege

The OS (Operating System) Account under which the DBMS (Database Management System) runs under should never be run as `root` or `system`. By default most DBMSs run as `system` account which is very powerful and dangerous. It is recommended to run the DBMS with an appropriate account with restricted privileges.

**To create a new user**:

Login to MySQL as root user
```bash
sudo mysql -u root -p
```

Now create a new user with restricted privileges using the following command
```MySQL
CREATE USER 'user_a'@'localhost' IDENTIFIED BY 'password';
GRANT SELECT ON mydatabase.* TO 'user_a'@'localhost';
FLUSH PRIVILEGES;
```

This way only the necessary privileges are to be assigned to the user account on the OS that runs the DBMS. To create new user application accounts on the database to be accessed from different devices, the `'localhost'` should be simply replaced by (`'%'`) meaning that it allows any host with the password to access the database which might be a threat if passwords are weak. So it is recommended to limit the access for IP range using `'192.13.32.%'`. This provides remote access and also maintaining security.

#### Additional Measures When Developing

When assigning privileges, if an application account needs access to only few tables, consider creating a `VIEW` that limits only to required data and providing access to only that view instead, rather that the actual table.

If you plan to use stored procedures everywhere, then do not allow application accounts to execute queries directly, rather only allow them to execute stored procedures that they require. Avoid giving them direct access to tables in the database.

#### Least Admin Privileges For Multiple Database User Accounts

The developers should refrain from using the same `root` or `admin` account for multiple web applications to connect to the database. Different application accounts are to be used for different web applications.

Each and every web application must have a designated database account with least privileges that it requires to connect to the database. This can help reduce the privileges to the web applications.

For example, consider a 'Login', 'Sign-Up' and 'Profile Page' web applications. Here providing `admin` or same account to all three web applications would be a poor decision. The 'Login' requires only `SELECT` access, whereas 'Sign-Up' requires only `INSERT` access and the 'Profile Page' application requires `SELECT` , `INSERT` and `UPDATE` access. Hence least privilege can be maximized.

#### Enhancing Least Privilege with SQL Views

You can use SQL views to enhance the granularity of access by limiting the read access to specific data of a table. For Example, consider storing password in plain-text instead of salty-hash values (due to specific legal requirements) in the table. If an application account is to be given access to the passwords, then a view is to be generated that depicts the hash values of the passwords. The access to the tables in the database is revoked from all the DB users except `admin`. 

If a successful SQL injection attack occurs, the attacker might have access to only salted-hash values of the passwords and not the plain-text passwords, Since no application user has access to the table itself.

#### Allow-List Input Validation

As mentioned earlier implementing Input Validation as a primary defense only if nothing else works, but implementing it as a secondary defense in highly recommended to improve security. As explained in above section, it is used to detect unauthorized input before appending to the query. Proceed with caution, Input validation is highly discouraged as a primary defense, unless it is the only option.

That's a wrap. Firstly, congratulations on making it so far. You have done a great job, wishing you Best of luck! 
