# 2021 Compilation Principle Lab2: LexicalGenerator
***This is a converter that allows for converting NFA to DFA, minimizing DFA, and so on.***  

---
## Requirements
Qt 5.9.9   
c++11

   
## Usages
The regular expression is entered in the editing box in the upper left corner, and the supported operations include: concatenation, parentheses, alternation (|), closure (*), and optional (?).   
The four buttons in the lower left corner are: import a .txt file containing regular expressions, generate analysis results, save the regular expression as a .txt file, and save the generated lexical program as a .txt file.   
On the right side, you can switch and view the NFA, DFA, minimized DFA, and the generated lexical program.

---
## 华南师范大学编译原理实验2——XLEX-词法自动生成器

***关于项目实现见最初版文件夹中的实验报告，以下是实验内容***

一、实验内容：   
设计一个应用软件，以实现将正则表达式-->NFA--->DFA-->DFA最小化-->词法分析程序

二、必做实验要求：   
 （1）正则表达式应该支持单个字符，运算符号有： 连接（|）、选择、闭包（*）、括号（）、可选（?  ）   
 （2）要提供一个源程序编辑界面，让用户输入一行（一个）或多行（多个）正则表达式（可保存、打开正则表达式文件）   
 （3）需要提供窗口以便用户可以查看转换得到的NFA（用状态转换表呈现即可）   
 （4）需要提供窗口以便用户可以查看转换得到的DFA（用状态转换表呈现即可）   
 （5）需要提供窗口以便用户可以查看转换得到的最小化DFA（用状态转换表呈现即可）   
 （6）需要提供窗口以便用户可以查看转换得到的词法分析程序（该分析程序需要用C/C++语言描述）   
 （7）用户界面应该是windows界面   
 （9）应该书写完善的软件文档   

三、选做实验要求：   
  （1）扩充正则表达式的运算符号，如   [ ] 、 正闭包（+） 等。   
  
四、完成时间：4周时间(第5周-第9周)   

五、上交方法：
通过砺儒云课堂提交   

六、完成方式：每个学生自行独立完成。   

七、实验实现的编程语言：C++程序设计语言   

八、实验需要提交的内容及注意事项：   

第2次实验作业提交，只能使用RAR文件或ZIP压缩文件。   

压缩文件内含文件夹及文件如下：   

（1）源程序文件夹：内含整个实验2的所有源程序文件和编译方法的说明介绍文件   

（2）文档文件夹：内含实验2的设计文档（PDF或DOC格式）（注：文档书写格式可参考百度云盘中课程实验文件夹下的格式）   

（3）测试数据文件夹：内含所有的测试数据文件(应该能测试所支持的每个运算符)和测试结果的汇报文件   

（4）可执行程序文件夹：内含实验2的可执行程序以及使用说明书。   





