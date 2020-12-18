# echo "1"
# ./mydisambig test_data/seg1.txt ZhuYin-Big5.map cor_lm ./test_data/bi1.out test_data/out_1
# ./mydisambig test_data/seg2.txt ZhuYin-Big5.map cor_lm ./test_data/bi2.out test_data/out_2
# ./mydisambig test_data/seg3.txt ZhuYin-Big5.map cor_lm ./test_data/bi3.out test_data/out_3
# ./mydisambig test_data/seg4.txt ZhuYin-Big5.map cor_lm ./test_data/bi4.out test_data/out_4
# ./mydisambig test_data/seg5.txt ZhuYin-Big5.map cor_lm ./test_data/bi5.out test_data/out_5
# ./mydisambig test_data/seg6.txt ZhuYin-Big5.map cor_lm ./test_data/bi6.out test_data/out_6
# ./mydisambig test_data/seg7.txt ZhuYin-Big5.map cor_lm ./test_data/bi7.out test_data/out_7
# ./mydisambig test_data/seg8.txt ZhuYin-Big5.map cor_lm ./test_data/bi8.out test_data/out_8
# ./mydisambig test_data/seg9.txt ZhuYin-Big5.map cor_lm ./test_data/bi9.out test_data/out_9
# ./mydisambig test_data/seg10.txt ZhuYin-Big5.map cor_lm ./test_data/bi10.out test_data/out_10

for ((i=1;i<=10;i++))
do
  echo "test bigram on file$i"
  #disambig -text "test_data/seg$i.txt" -map "ZhuYin-Big5.map" -lm "cor_lm" -order "3" > "test_data/out3_$i"
  ./mydisambig "test_data/seg$i.txt" "ZhuYin-Big5.map" "cor_lm" "./test_data/bi$i.out"
  echo "-------------------------------"
done