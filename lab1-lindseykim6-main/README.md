# Lindsey Kim 
## CS50 Spring 2021, Lab 1

GitHub username: lindseykim6

## B
``` bash
$ f004r97@plank:~/cs50-dev/lab1-lindseykim6$ ln -s ~/cs50-dev/shared/COVID-19/data_tables/vaccine_data/us_data/hourly/vaccine_data_us.csv vaccine_data_us.csv
$ 
```
## C
```bash
$ head -1 vaccine_data_us.csv
$
```

## D
```bash
$ head -1 vaccine_data_us.csv | tr [,] '\n'
$
```

## E
``` bash
$ grep "New Hampshire" vaccine_data_us.csv 
$
```

## F
``` bash
$ grep "All" vaccine_data_us.csv|cut -d ',' -f 2,10
$
```

## G
``` bash
$ grep "All" vaccine_data_us.csv|cut -d ',' -f 2,10| sort -t ',' -nr -k2|head
$
```

## H
```
$ grep "All" vaccine_data_us.csv|cut -d ',' -f 2,10| sort -t ',' -nr -k2|head|sed 's/,/|/'| sed 's/^/|/'| sed 's/$/|/'
$
```

