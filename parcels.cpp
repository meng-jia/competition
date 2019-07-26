/*
//C++
Created on Thu Jul 26 20:08:30 2019
@author: jiadeng
*/

#include<iostream>//cause cin,cout
#include<cstring>//cause memset
#include<algorithm>// cause max,min
#include<limits.h>// cause INT_MIN,INT_MAX
#include<queue> // cause queue<>

using namespace std;

const int N = 255;
int dist[N][N];
string g[N];
int n,m;


//bfs()函数功能:从全部的delivery office 即‘1’出发
//遍历所有距离这delivery offices小于等于k的square坐标
//更新dist[N][N]
void bfs( int k)
{
    queue<pair<int,int>> q;
    
    //dist全部初始化为 -1
    memset (dist, -1 , sizeof(dist));
    
    for(int i = 0; i < n; i ++)
    {
        for(int j = 0; j < m; j ++)
        {   
            if (g[i][j] == '1')//如果是delivery office
            {
                dist[i][j] = 0;//将dist[i][j]置为0
                q.push({i,j});//把{i,j}入队列q
            }
        }
    }
    //一次移动一步，上下左右，行数和列数的值变化
    int dc[4]={-1,1, 0, 0};
    int dr[4]={ 0,0,-1, 1};
    
    while(q.size())
    {
        //取出队首元素,其类型是pair,横、纵坐标组成pair
        auto q_front = q.front();//注意这个auto
       
        //【注意】删除队首元素，遍历过的square坐标要删除
        q.pop();
        
        //分别取出队首元素的横、纵坐标
        int q_front_x = q_front.first, q_front_y = q_front.second; 
        
        //队首元素的dist
        int distance = dist[q_front_x][q_front_y];
        
        //如果队首元素与delivery office的距离等于k，不再遍历该元素的附近
        if (distance == k) continue; 
        //遍历该队首元素的neighbor（即上下左右）
        for(int i = 0; i < 4; i ++)
        {
            int q_front_neighbor_x = q_front_x + dc[i];
            int q_front_neighbor_y = q_front_y + dr[i];
            //队首元素的neighbor的横、纵坐标在边界之内，并且没有被访问过
            if (q_front_neighbor_x>=0 &&
                q_front_neighbor_x<n  &&
                q_front_neighbor_y>=0 &&
                q_front_neighbor_y<m  &&
                dist[q_front_neighbor_x][q_front_neighbor_y] == -1)
            {
                //更新dist值 
                dist[q_front_neighbor_x][q_front_neighbor_y] = distance + 1;
                //将队首元素的neighbor入队，等待遍历队首元素的neighbor的neighbor
                q.push({q_front_neighbor_x, q_front_neighbor_y});
            }    
        }
    }
}

//check()函数功能：check是否能取到overall delivery time为 k
//after adding at most one additional delivery office
bool check(int k)
{
    bfs(k);
    int sum_max = INT_MIN, sum_min = INT_MAX;
    int sub_max = INT_MIN, sub_min = INT_MAX;
   //(x1,y1)和(x2,y2) 的 Manhattan distance如下
   //max(abs(  横纵坐标和之差）,    abs(   横纵坐标差之差)   )
   //max(abs( (x1+y1) - (x2+y2) ) , abs( (x1-y1) - (x2-y2) ) )
    for(int i = 0; i < n; i ++)
    {
        for(int j = 0; j < m; j ++)
        {
            //没有被遍历的square坐标
            //即距离delivery offices大于k的square坐标
            if (dist[i][j] == -1)
            {
                sum_max = max( sum_max, i + j);
                sum_min = min( sum_min, i + j);
                sub_max = max( sub_max, i - j);
                sub_min = min( sub_min, i - j);
            
            }
        }
    }
    if (sum_max == INT_MIN) return true;
    for(int i = 0; i < n; i ++)
    {
        for(int j = 0; j < m; j ++)
        {
            if (g[i][j] == '0')
            {
                int sum = max( abs(sum_max - (i + j)) , abs(sum_min - (i + j)));//注意括号层数
                int sub = max( abs(sub_max - (i - j)) , abs(sub_min - (i - j)));//注意括号层数
                if (max(sum,sub) <= k) return true;        
            }
        }
    }
    return false;
}
int main()
{
    int T;
    cin >> T;
    for (int C=1; C<=T; C++)
    {
        //【特别注意】，不要写成了 cin >> n,m; 
        cin >> n >> m;//对应输入的 R, C
        for (int i = 0; i < n; i ++) cin >> g[i]; //读入a string of C characters
        //下面是二分法的标准模板
        // overall delivery time 最小是0， 最大是 m+n,也就是行号+列号
        int mid, l = 0, r = m+n;
        while(l!=r)
        {

            mid = l + r >> 1;//【特别注意】不能少了这行
            
            //overall delivery time能取到mid，再进行二分
            //看更小overall delivery time能否取到
            if (check(mid)) r = mid;
            
            //overall delivery time不能取到mid，再进行二分
            //看更大overall delivery time能否取到
            else l = mid + 1;
        }
        //while结束，取到 minimum overall delivery time 
        printf("Case #%d: %d\n", C, l);    
    }
    return 0;
}

/*
The author would like to express gratitude to 
yxc's contribution on 
https://www.bilibili.com/video/av47610017
*/
