#!/usr/bin/perl

use Pg;
use strict;

my $connection;
#For SQL call.
my $sql;

my $result;

#Date and Time of data.
my $timestamp;

#Outside temperature.
my $out_temperature;

#Outside Humidity.
my $out_humidity;

#Wind Speed.
my $wind_speed;

#Direction of Wind.
my $wind_heading;

#The requested month by user.
my $usermonth = "january";

#The requested year by user.
my $useryear = '2002';

#The end of the month requested.
my $monthtill;

#Counter used somewhere here.
my $counter = 0;

#Output file containing data to be graphed.
my $outputfile = "tograph.dat";


#System calls to delete any existing outputfiles, and the create a new one.
system("rm $outputfile");
system("touch $outputfile");

#Start HTML enteries.
print "<html>\n";
print "<body>\n";

#Check whether or not the user has entered a month.
if ( !$usermonth ) {		
    print "You did not specify a valid month. Please try again.\n";
}

#Check whether or not the user has entered a year.
if ( !$useryear ) {
    print "You did not specify a valid year. Please try again.\n";
}


#Check if the user has entered a valid month or not.
if ( $usermonth != "january" && $usermonth != "february" && $usermonth != "march"
    && $usermonth != "april" && $usermonth != "may" && $usermonth != "june"
    && $usermonth != "july" && $usermonth != "august" && $usermonth != "september"
    && $usermonth != "october" && $usermonth != "november" && $usermonth != "december" ) {
	
        print "Please specify a valid month";
    }


#Now assigned the timestamps for requested months. Set the starting and the ending points.
if ( $usermonth eq "january" ) {
    $usermonth = "$useryear-01-01 00:00:00-05";
    $monthtill = "$useryear-01-30 23:59:59-05";
}

if ( $usermonth eq "february" ) {
    $usermonth = "$useryear-02-01 00:00:00-05";
    $monthtill = "$useryear-02-27 23:59:59-05";
}

if ( $usermonth eq "march" ) {
    $usermonth = "$useryear-03-01 00:00:00-05";
    $monthtill = "$useryear-03-30 23:59:59-05";
}

if ( $usermonth eq "april" ) {
    $usermonth = "$useryear-04-01 00:00:00-05";
    $monthtill = "$useryear-04-30 23:59:59-05";
}

if ( $usermonth eq "may" ) {
    $usermonth = "$useryear-05-01 00:00:00-05";
    $monthtill = "$useryear-05-30 23:59:59-05";
}

if ( $usermonth eq "june" ) {
    $usermonth = "$useryear-06-01 00:00:00-05";
    $monthtill = "$useryear-06-30 23:59:59-05";
}

if ( $usermonth eq "july" ) {
    $usermonth = "$useryear-07-01 00:00:00-05";
    $monthtill = "$useryear-07-30 23:59:59-05";
}

if ( $usermonth eq "august" ) {
    $usermonth = "$useryear-08-01 00:00:00-05";
    $monthtill = "$useryear-08-30 23:59:59-05";
}

if ( $usermonth eq "september" ) {
    $usermonth = "$useryear-09-01 00:00:00-05";
    $monthtill = "$useryear-09-30 23:59:59-05";
}

if ( $usermonth eq "october" ) {
    $usermonth = "$useryear-10-01 00:00:00-05";
    $monthtill = "$useryear-10-30 23:59:59-05";
}

if ( $usermonth eq "november" ) {
    $usermonth = "$useryear-11-01 00:00:00-05";
    $monthtill = "$useryear-11-30 23:59:59-05";
}

if ( $usermonth eq "december" ) {
    $usermonth = "$useryear-12-01 00:00:00-05";
    $monthtill = "$useryear-12-30 23:59:59-05";
}


print "Content-Type: text/html\n\n";

#Connect to the Postgres DataBase.
$connection = Pg::connectdb("dbname=weather");

#Issue an error message if conection fails.
if ($connection->status ne PGRES_CONNECTION_OK) {
    print "<h4>Error retrieving weather data (connectdb), ", $connection->errorMessage, "</h4>\n";
    exit 0;
}

#The SQL statement to be retrieved into the DataBase.
$sql = "select timestamp, out_temperature, out_humidity, wind_speed, wind_heading " .
  "from readings " .
  "where timestamp > '$usermonth' and timestamp < '$monthtill'; ";

#Create an HTML table to display the Monthly Data.
print"<table border=0>\n";
print"<tr bgcolor=#cccccc>\n";
print"  <td width=20>Time/Date</td>\n";
print"  <td width=20>Temperature</td>\n";
print"  <td width=20>Humidity</td>\n";
print"  <td width=20>Wind Speed</td>\n";
print"  <td width=20>Wind Heading</td>\n";
print"</tr>\n";

#Execute the SQL statement into DataBase now.
$result = $connection->exec($sql);

#If an error occurs issue an error statement.
if ($result->resultStatus ne PGRES_TUPLES_OK) {
    print "<h4>Error retrieving weather data (exec), ", $connection->errorMessage, "</h4>\n";
    exit 0;
}

#Get the data now and Apply into HTML.
while ( ( ($timestamp, $out_temperature, $out_humidity, $wind_speed, $wind_heading) = $result->fetchrow) ) {
    
    $counter++;
    
    #Insert data into file to graph.
    system("echo $counter $out_temperature >> $outputfile");
    
    #Check for Wind Direction.
    my $heading;
    if ($wind_heading > 337.5 || $wind_heading <= 22.5) {
	$heading = "N";
    } elsif ($wind_heading > 22.5  && $wind_heading <= 67.5) {
	$heading = "NE";
    } elsif ($wind_heading > 67.5  && $wind_heading <= 112.5) {
	$heading = "E";
    } elsif ($wind_heading > 112.5 && $wind_heading <= 157.5) {
	$heading = "SE";
    } elsif ($wind_heading > 157.5 && $wind_heading <= 202.5) {
	$heading = "S";
    } elsif ($wind_heading > 202.5 && $wind_heading <= 247.5) {
	$heading = "SW";
    } elsif ($wind_heading > 247.5 && $wind_heading <= 292.5) {
	$heading = "W";
    } elsif ($wind_heading > 292.5 && $wind_heading <= 337.5) {
	$heading = "NW";
    } else {
	print "<h4>Error: illegal wind heading: $wind_heading</h4>\n";
    }
    
    #Celcuis and F. calculations.
    my $out_temp_F = $out_temperature * 9/5 + 32;
    $out_temperature = sprintf("%3.0f", $out_temperature);
    $out_temp_F      = sprintf("%3.0f", $out_temp_F);
    
    #Wind Speed calculations.
    my $wind_speed_mph = $wind_speed / 0.447;
    $wind_speed_mph = sprintf("%2.0f", $wind_speed_mph);
    
    #If an error occurs, issue an error statement into HTML, else plug into HTML table.
    if ($result->resultStatus ne PGRES_TUPLES_OK) {
	print "<h4>Error retrieving weather data (fetchrow), ", $connection->errorMessage, "</h4>\n";
    } else {
	my $html = << "_END_";
	<tr>
	  <td align=center>$timestamp</td>
	  <td align=center>$out_temperature C ($out_temp_F F)</td>
	  <td align=center>$out_humidity\%</td>
	  <td align=center>$wind_speed m/s </td>
	  <td align=center>$heading ($wind_speed_mph mph)</td>
	  </tr>
	  
_END_
	  print "$html\n";
    }
    
}

#Issue last HTML statements.
print " </table>\n";
print " </body>\n";
print " </html>\n";

#Exit successfully.
exit 0;
