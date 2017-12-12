# 下面是MarkDown测试

[TOC]


### 1、标题

```
标题
# Title1
## Title2
### Title3
#### Title4
##### Title5
###### Title6
```

# Title1
## Title2
### Title3
#### Title4
##### Title5
###### Title6

### 2、分割线
```
******
------
```

******
------

### 3、列表

有序列表


1. list1
2. list2
        1. list3
        2. list4
                1. list5
3. list6


无序列表


* list1
+ list2
        - list3
        * list4
            + list5
- list6

### 4、修饰

> 使用一对*倾斜
这是* 倾斜 *。
> 使用一对**加粗
这是** 加粗 **。
> 使用一对～～来删除
这是~~ 删除 ~~。
> 使用 `来添加行内代码
这是```printf```。
> 使用一对 $ 来引用 Latex代码
$ A_v = \frac{-\beta * R_C}{h_{ie}} $

### 5、图片和链接
#### 导入图片


![baidu](baidu.gif)


#### 导入链接

[百度一下](http://www.baidu.com)


#### 导入角标

这是共模抑制比[CMRR][1]


### 6、引用、代码

可以使用">"来引用，可以递归
```
> 引用
>> 二级引用
>> 二级引用
>>> 三级引用
> 回到一级引用
```
> 引用
>> 二级引用
>> 二级引用
>>> 三级引用
> 回到一级引用


#### 也可以使用行间代码
``` C
#include<iostream>
using namespace std;
int main(){
    cout<<"Hello,World"<<endl;
}
```

### 7、表格

#### 例如如下表格
```
|姓名|学号|班级|GPA|
|:-—|:-:|:-:|--:|
|谈健|001|03|4.3|
|凌志|002|03|4.3|
```

|姓名|学号|班级|GPA|
|:---|:--:|:--:|--:|
|谈健|001|03|4.3|
|凌志|002|03|4.3|

### 8、TOC

通过"[TOC]"标签，根据标题内容自动生成 内容表TOC和页面内链接。

### 9、HTML

可以在块之外自由使用HTML标签（不推荐），此时需要使用\ 转义字符来修饰<和&。
```
\ <i> 使用html倾斜标签 \ </i>

\ <p style="font-family:arial;color:red;font-size:20px;">测试复杂html属性\ </p>
```

\<i>使用html倾斜标签\</i>

\<p style="font-family:arial;color:red;font-size:20px;">测试复杂html属性.\</p>

### 10、流程图

以[flow]开始流程图，以[!flow] 结束

> [flow]
> st=>start: 开始
> e=>end: 结束
> op=>operation: 一只谈健
> cond=>condition: 帅么
> st->op->cond
> cond(yes)->op
> cond(no)->e
> [!flow]


[flow]

st=>start: 开始
e=>end: 结束
op=>operation: 一只谈健
cond=>condition: 帅么
st->op->cond
cond(yes)->op
cond(no)->e

[!flow]


end of file
