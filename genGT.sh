for ((i=1;i<=10;i++))
do
  #echo "aa$i"
  disambig -text "test_data/seg$i.txt" -map "ZhuYin-Big5.map" -lm "cor_lm" -order "3" > "test_data/out3_$i"
done