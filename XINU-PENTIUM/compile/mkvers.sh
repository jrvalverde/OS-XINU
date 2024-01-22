if [ -f vn ]; then
	expr `cat vn` + 1 > vn
else
	echo 0 > vn
fi
USER=`whoami`
HOST=`hostname`
HOST=`basename $HOST .purdue.edu`
VN=`cat vn`
echo "#$VN ("$USER"@"$HOST") "`date` > version
