for l in `ls -1 /peek/$1/tcp_conn_table/`
do
  printf "$l\t:\t"
  cat /peek/$1/tcp_conn_table/$l/tc_locaddr | xargs -n 1 inttoip | tr '\n' ' '
  cat /peek/$1/tcp_conn_table/$l/tc_locport | xargs -n 1 endianswap | tr '\n' ' '
  printf "  ->  "  
  cat /peek/$1/tcp_conn_table/$l/tc_remaddr | xargs -n 1 inttoip | tr '\n' ' '
  cat /peek/$1/tcp_conn_table/$l/tc_remport | xargs -n 1 endianswap 
done
