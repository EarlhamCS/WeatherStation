select timestamp, out_temperature, wind_speed, wind_heading from
readings where timestamp > 'Sep 15 00:00:00 EST 2001'
and timestamp < 'Oct 10 23:59:00 EST 2001' ;
