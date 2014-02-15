#!/usr/local/bin/perl

use Pg;
use strict;

my $username = "weather";          # The local and remote user name.

my $line;

my $in_temperature;
my $out_temperature;
my $wind_speed;
my $wind_heading;
my $barometric_pressure;
my $barometric_trend;
my $in_humidity;
my $out_humidity; 
my $rain;
my $wind_chill;
my $dew_point;
my $debug = 0; 

if ($debug) { print "before date\n"; }
my $timestamp = `date`;
chop $timestamp;
if ($debug) { print "after date\n"; }

my $connection;
my $result;
my $sql_statement;

# Kill off any blocked ./getWeather processes.
if ($debug) { print "before rsh ps\n"; }
open(data, "rsh proto.cs.earlham.edu ps auxw | grep ./getWeather|");
if ($debug) { print "after rsh ps\n"; }
while(<data>) {
    my @words = reverse(split);
    while(pop(@words) ne $username) { }
    my $pid = pop(@words);
    if ($debug) { print "before rsh kill\n"; }
    system("rsh proto.cs.earlham.edu kill -9 $pid");
    if ($debug) { print "after rsh kill\n"; }
}
close(data);

my %weather;
if ($debug) { print "before rsh getWeather.pl\n"; }
open(data, "rsh proto.cs.earlham.edu ./getWeather|");
if ($debug) { print "after rsh getWeather.pl\n"; } 
while(<data>) {
    chomp;
    my ($label, $value) = split(" : ");
    $label =~ s/\s+$//;               # Trim trailing white space.
    $value =~ s/^\s+//;               # Trim leading white space.
    $weather{$label} = $value;
#    print "$label : $value\n";
}
close(data);

$in_temperature      = $weather{"Current Inside Temp [C]"};
$out_temperature     = $weather{"Current Outside Temp [C]"};
$wind_speed          = $weather{"Current Wind Speed [m/s]"};
$wind_heading        = $weather{"Current Wind Direction [deg]"};
$barometric_pressure = $weather{"Current Pressure [mm Hg]"};
$barometric_trend    = $weather{"Barometric Trend"};
$in_humidity         = $weather{"Current Inside Humidity [%]"};
$out_humidity        = $weather{"Current Outside Humidity [%]"}; 
$rain                = $weather{"Total Rain [cm]"};
$wind_chill          = $weather{"Current Wind Chill [C]"};
$dew_point           = $weather{"Current Dew Point [C]"};

#print "in_temperature = $in_temperature\n";
#print "out_temperature = $out_temperature\n";
#print "wind_speed = $wind_speed\n";
#print "wind_heading = $wind_heading\n";
#print "barometric_pressure = $barometric_pressure\n";
#print "barometric_trend = $barometric_trend\n";
#print "in_humidity = $in_humidity\n"; 
#print "out_humidity = $out_humidity\n";
#print "rain = $rain\n"; 
#print "wind_chill = $wind_chill\n"; 
#print "dew_point = $dew_point\n"; 

$connection = Pg::connectdb("dbname=weather") or 
   die "Pg::connectdb failed, ", $connection->errorMessage; 

$sql_statement = "insert into readings values " .
   "('$timestamp', $in_temperature, $out_temperature, $wind_speed, " .
   " $wind_heading, $barometric_pressure, $in_humidity, " .
   " $out_humidity, $rain, $wind_chill, $dew_point, '$barometric_trend')"; 

$result = $connection->exec("$sql_statement");
die $connection->errorMessage, "\n\nConstraints:\n1-Inside Temp. (0 <= x <= 50)\n".
			       "2-Outside Temp. (-30 <= x <= 50)\n".
			       "3-Wind Speed. (0 <= x <= 100)\n". 
			       "4-Wind Heading. (0 <= x <= 360)\n".
			       "5-Barometric pressure (72000 <= x <= 76000)\n".
			       "6-Inside Humidity. (0 <= x <= 100)\n".
			       "7-Outside Humidity. (0 <= x <= 100)\n".
			       "8-Rain. (0 <= x <= 100)\n".
			       "9-Wind Chill. (-40 <= x <= 50)\n".
			       "10-Dew Point. (-35 <= x <= 50)\n".
			       "\nWeather Data:\n\n Inside temperature: $in_temperature".
			       "\n Outsise temperature: $out_temperature\n Wind Speed: $wind_speed".
			       "\n Wind Heading: $wind_heading\n Barometric pressure: $barometric_pressure".
			       "\n Barometric Trend: $barometric_trend\n Inside Humidity: $in_humidity".
			       "\n Outside Humidity: $out_humidity\n Rain: $rain\n Wind Chill: $wind_chill".
			       "\n Dew Point: $dew_point\n" unless PGRES_COMMAND_OK eq $result->resultStatus;
exit;
