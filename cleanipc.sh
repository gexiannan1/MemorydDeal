#用于清除共享内


for i in `ipcs |grep gexiannan |awk  '{print $2}'`
do
  ipcrm -m $i
  ipcrm -s $i
done
