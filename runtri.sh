for ((i=1;i<=10;i++))
do
  echo "test trigram on file$i"
  #disambig -text "test_data/seg$i.txt" -map "ZhuYin-Big5.map" -lm "cor_lm" -order "3" > "test_data/out3_$i"
  ./mydisambig "test_data/seg$i.txt" "ZhuYin-Big5.map" "cor_lm" "./test_data/tri$i.out" "--tri"
  echo "-------------------------------"
done