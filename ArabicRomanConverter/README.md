# Arabic-Roman Converter - Project description

A web application that converts user input between Arabic numerals (standard numerical numbers) and Roman numerals. 
It supports both conversions: Arabic to Roman and Roman to Arabic.

The application maintains a conversion history and uses cookies to store user data. 
Additionally, all conversion results are stored in a database and can be displayed to the user.


# Before running the solution

1. Open the Project

Use NetBeans IDE 22 or a later version to open the project.

2. Configure the Database

In the Services tab, expand Databases and right-click Java DB → Properties.

Set both Java DB Installation and Database Location to the following path:
(Your download path)..\ArabicRomanConverter\src\main\resources\db-derby-10.14.2.0-bin\db-derby-10.14.2.0-bin

Right-click the database named jdbc:derby://localhost:1527/sample → Connect.

If the database is set up properly, a database named APP with default tables should be created and visible.

3. Set Up the Server

In the Servers section, right-click → Add Server → Payara Server.
Install it using the default settings, then start the server.

4. Build and Run the Project

In the Projects tab, build the project and then run it to open the web application.

Enjoy using the application!


