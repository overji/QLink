# QLink项目编写规范
## 1.行与列
boxM代表总行数，boxN代表总列数     
QPair规定为(行，列)   
描述箱子box坐标时不得使用X,Y之类易造成混淆的词语     
应当使用row,col代替       
使用LinkGame的boxMap时，应当为`boxMap[行][列]`

## 2.分数
type1 ~ type7:
<table>
<thead>
<th>类型</th>
<th>概率</th>
<th>得分</th>
</thead>
<tbody>
<tr>
<td>1</td>
<td>25%</td>
<td>10</td>
</tr>
<tr>
<td>2</td>
<td>22.5%</td>
<td>20</td>
</tr>
<tr>
<td>3</td>
<td>15%</td>
<td>30</td>
</tr>
<tr>
<td>4</td>
<td>12.5%</td>
<td>50</td>
</tr>
<tr>
<td>5</td>
<td>10%</td>
<td>75</td>
</tr>
<tr>
<td>6</td>
<td>10%</td>
<td>100</td>
</tr>
<tr>
<td>7</td>
<td>5%</td>
<td>200</td>
</tr>
</tbody>
</table>

3.主界面
16列12行，50*50的网格
