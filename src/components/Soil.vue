<template>
    <div class="com-container" >
      <div class="com-chart" id="myChart2"  >
        <!-- <div class="com-chart" id="myChart" :style="{ width: '800px', height: '500px' }" ></div> -->
   
      </div>
    </div>
  </template>
  
  <script>
  // 这个是局部引用 this.$echarts。全局已经挂在，使用方法echarts,只不过有时候报错，我这里就先用局部代替一下
  import * as echarts from 'echarts'
  // json 后缀不可省略
  import purple from '../../public/static/theme/purple-passion.json'
  
  
  export default {
      data(){
          return {
  
              chartInstance:null
          }
      },
      mounted()
      {
     
      this.drawCharts();
      
      },
      methods:{
   
  
  
          //对比黑马视频51
          //获取后台端口的数据
          drawCharts(){
              let self=this
               // 基于准备好的dom，初始化echarts实例,init的第二个参数为主题
              this.$echarts.registerTheme("purple",purple)//注册主题
               let myChart = this.$echarts.init(document.getElementById("myChart2"),'purple');
              //获取数据
              self.$axios.get('soilrouter')
              .then(res=>{
                  const all_data=res.data
                  console.log(all_data)
                 
                  const y_temp=[]
                  const y_electricity=[]
                  const y_salt=[]
  
                  
                  for(var i=0;i<all_data.length;i++)
                  {
                      y_temp.push(all_data[i].temp)
                      y_electricity.push(all_data[i].electricity)
                      y_salt.push(all_data[i].salt)

                  }
                  console.log('y_temp:',y_temp)
                  console.log('y_electricity:',y_electricity)
                  console.log('y_salt:',y_salt)

                

                  myChart.setOption( {
  title: {
    text: '土壤数据'
  },
  tooltip: {
    trigger: 'axis'
  },
  legend: {},
  toolbox: {
    show: true,
    feature: {
      dataZoom: {
        yAxisIndex: 'none'
      },
      dataView: { readOnly: false },
      magicType: { type: ['line', 'bar'] },
      restore: {},
      saveAsImage: {}
    }
  },
  xAxis: {
    type: 'category',
    boundaryGap: false,
    data:['1','2','3','4','5','6','7','8','9','10']
  },
  yAxis: {
    type: 'value',

  },
  series: [
    {
      name: '含盐量',
      type: 'line',
      data: y_salt,
      markPoint: {
        data: [
          { type: 'max', name: 'Max' },
          { type: 'min', name: 'Min' }
        ]
      },
      markLine: {
        data: [{ type: 'average', name: 'Avg' }]
      }
    },
    {
      name: '电导率%',
      type: 'line',
      data: y_electricity,
      markPoint: {
        data: [
          { type: 'max', name: 'Max' },
          { type: 'min', name: 'Min' }
        ]
      },
      markLine: {
        data: [{ type: 'average', name: 'Avg' }]
      }
    },
    {
      name: '土壤温度',
      type: 'line',
      data: y_temp,
      markPoint: {
        data: [
          { type: 'max', name: 'Max' },
          { type: 'min', name: 'Min' }
        ]
      },
      markLine: {
        data: [{ type: 'average', name: 'Avg' }]
      }
    },
  ]
}
                  )
    
  
  
      //设置回调函数 ，让页面隔一段时间刷新一下
      setTimeout(()=>{
          self.drawCharts()
      },2000)
        });
  
              
          },
          
          
      }
  
  }
  </script>
  
  <style>
  
  </style>