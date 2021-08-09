# 2021 CAD Contest Problem F: Verilog Simulation Optimization via Instruction Reduction
  >這份程式是 _**2021 CAD Contest Problem F(推廣題)**_ 的參考作法，關於題目的詳細敘述可以到2021 CAD Contest的網站查看(http://iccad-contest.org/2021/tw/)

### 檔案名稱
  * main.cpp是主程式
  * ex_pic1、ex_pic2、ex_pic3和ex_pic4是用來作範例的圖片

### 使用方式
  1. 先將.cpp檔案編譯成.exe檔
  2. 到powershell中輸入./exe檔名 讀取檔名 輸出檔名

### 使用範例
  1. 假設在桌面有一個叫做example的資料夾，資料夾裡有main.cpp(主程式)和original.v(測試資料)<br>
  ![ex_pic1](https://github.com/tplin1999/2021_CAD_Contest_Problem_F/blob/main/ex_pic1.png)
  
  2. 先用VScode將main.cpp編譯成main.exe<br>
  ![ex_pic1](https://github.com/tplin1999/2021_CAD_Contest_Problem_F/blob/main/ex_pic2.png)
  
  3. 再到powershell輸入"cd Desktop\example"，以及"./main.exe original.v original_ans.v"<br>
  ![ex_pic1](https://github.com/tplin1999/2021_CAD_Contest_Problem_F/blob/main/ex_pic3.png)
  
  4. 即可獲得original_ans.v 這個檔案<br>
  ![ex_pic1](https://github.com/tplin1999/2021_CAD_Contest_Problem_F/blob/main/ex_pic4.png)
