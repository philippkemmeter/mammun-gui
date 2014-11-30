#!/bin/bash

for lng in `ls`;
do
	case $lng in
		*.*) ;;
		*)
			cd $lng/ini
			for f in `ls *`;
			do
				out=$(php -r "parse_ini_file(\"$f\");" 2> /dev/null);
				if [[ "$out" != "" ]]
				then
					echo -n "Fehler! Datei $f konnte nicht verarbeitet werden. "
					echo "PHP gab folgenden Fehler aus:"
					echo $out
					cd ../../
					exit 1
				else
					echo -n "."
				fi
				
			done
			cd ../../
		;;
	esac
done

echo "Alle ini-Dateien sind gut :)"
exit 0
