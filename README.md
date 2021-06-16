# C语言大作业





## 1.使用的知识点分析



###  	·简单操作符

​		+ , -  , * , / , ++ , -- , &

### ·简单关键字

​		if ，else ， switch .. case , goto , int , float ...

### ·知识点

#### 1.基本数据类型的使用。

#### 2.if else , for , while等基本关键字的使用

#### 3.指针的使用

#### 4.动态分配内存

#### 5.双向链表的使用

#### 6.EasyX 图像库的使用

#### 7.多线程以及线程同步



## 2.界面分析



### 初始界面

![https://github.com/False-Mask/CLanguageFinalExam\pics\image-20210616125711615.png)

#### 简述

点击开始游戏既可打开游戏，进入游戏界面。

点击退出游戏即可退出。

#### 实现原理

利用EasyX库开辟一个窗口，调用其画图的api初始化图形界面。又通过其提供的getMsg对鼠标操作进行处理。





### 游戏界面

![https://github.com/False-Mask/CLanguageFinalExam\pics\image-20210616125801868.png)

#### 简述

WSAD分别对应上下左右空格对应发射子弹，子弹击中敌人，便可减少敌人血量，血量为0则死亡。其中小型飞机血量为1，中型飞机血量为2，大型飞机血量为5。于此同时敌方战机如果碰到了我方飞机也会扣除对应的血量。很不幸的是，我方飞机的血量只有1。



#### 实现原理

通过开辟3个线程对事件进行处理。一条对UI界面进行刷新，一条对用户的键盘输入进行获取，一条对游戏的数据进行处理（如子弹的位置刷新，子弹的回收，敌军位置的刷新，敌军数量的控制）。

##### 	1. UI线程

​	UI线程实现的只是UI界面的刷新，其中我将一些必要的素材声明为全局。当我方战机还存活的情况下不断进行刷新。

##### 	2.检测键盘输入的线程

​	作用很单一，当我方战机存活的情况下不断监控键盘是否有输入。如果过有就切换到对应的switch -- case 分支。

##### 	3.数据处理线程

​	这个线程进行了比较多的逻辑运算。依然也是以我方战机的存货情况为判断条件不断循环检测，刷新。检测内容主要分敌军是否与我军相碰，敌军是否被子弹击中，敌军击中后是否死亡。每次检测完成之后就会对敌军(双向链表的位置进行更新)，与此同时还会对子弹的位置进行刷新。最后还会对子弹和敌军是否逃离的屏幕的边界。如果远离了边界，那么就将双向链表所对应位置的内容进行删除。









死亡界面

![https://github.com/False-Mask/CLanguageFinalExam\pics\image-20210616125849458.png)





#### 简述

无（死都死了有什么好说的呢）





## 3.其中所遇到的问题



### ·开辟线程的问题

由于我使用的EasyX库，这个库是C++写的所以我必须得用cpp文件写c代码。但是visual studio并不支持c语言的多线程库pthread,于此同时pthread还与windows平台有不兼容的问题，还有pthread的内部的结构体竟然和time.h内部的结构体有重定义的问题（这就很离谱）。最后我选择使用的iostream也就是c++的标准库内的线程api，进行的多线程处理。

### ·多线程对全局变量处理的原子性问题

这说实话让我当时非常的崩溃。我在对无用数据free的时候老是会出现0xDDDDDD的错误（也就是对正在使用的数据free导致指针的指向不确定）之后我对数据线程加了一把线程锁。当对数据进行操作就去获取锁，操作完成就释放。于此同时UI线程使用数据就获取锁，图像界面刷新完毕就释放锁，这样勉勉强强才把这垃圾界面打出来。既避免了非原子操作，又避免了UI刷新的卡顿。

#### ·赋值不规范，亲人两行泪

每次malloc内存以后就很少会赋空值，这也导致我建立双向链表的时候到处都是一些奇奇怪怪的问题。规范使用以后得以延缓。

#### ·无处不在的NULL POINTER

磨人的空指针。起初开发的时候，习惯很差基本上不会对指针为空进行判断，这也导致每次出bug就是一片的空指针。最后渐渐的规范的使用，空指针的问题才得以延缓。





## 4.收获

收获最大的还是开发经验，之前从来没想过竟然能用c语言写一个GUI，EasyX库还是比较强大。这也让我进一步认识到了库的重要性，没有库函数我就是个Rubish。于此同时我对指针的畏惧更加深刻了，太危险了，使用不当就是满篇的错误，让人非常的抓狂。最后就是编程规范，很多约定的编程规范总是被我抛在脑后，现在出了磨人的bug后我才意识到良好编程风格的重要性。良好的编程风格不仅仅会让你的代码看上去舒服，bug都会少很多。