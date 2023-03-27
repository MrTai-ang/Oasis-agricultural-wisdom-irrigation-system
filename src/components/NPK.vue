<template>
    <div class="com-container" >
      <div class="com-chart" id="myChart1"  >
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
               let myChart = this.$echarts.init(document.getElementById("myChart1"),'purple');
              //获取数据
              self.$axios.get('npkrouter')
              .then(res=>{
                  const all_data=res.data
                  console.log(all_data)
                 
                  var y_N=all_data[0].N
                  var y_P=all_data[0].P
                  var y_K=all_data[0].K
  
                  
               
                  console.log('N:',y_N)
                  console.log('P:',y_P)
                  console.log('K:',y_K)


 const gaugeData = [
  {
    value: y_N,
    name: 'N mg/kg',
    title: {
      offsetCenter: ['-70%', '80%']
    },
    detail: {
      offsetCenter: ['-70%', '115%']
    }
  },
  {
    value: y_P,
    name: 'P mg/kg',
    title: {
      offsetCenter: ['0%', '80%']
    },
    detail: {
      offsetCenter: ['0%', '115%']
    }
  },
  {
    value: y_K,
    name: 'K mg/kg',
    title: {
      offsetCenter: ['70%', '80%']
    },
    detail: {
      offsetCenter: ['70%', '115%']
    }
  }
];

                  myChart.setOption( {
                    series: [
    {
      type: 'gauge',
      anchor: {
        show: true,
        showAbove: true,
        size: 18,
        itemStyle: {
          color: '#FAC858'
        }
      },
      pointer: {
        icon: 'path://M2.9,0.7L2.9,0.7c1.4,0,2.6,1.2,2.6,2.6v115c0,1.4-1.2,2.6-2.6,2.6l0,0c-1.4,0-2.6-1.2-2.6-2.6V3.3C0.3,1.9,1.4,0.7,2.9,0.7z',
        width: 8,
        length: '80%',
        offsetCenter: [0, '8%']
      },
      progress: {
        show: true,
        overlap: true,
        roundCap: true
      },
      axisLine: {
        roundCap: true
      },
      data: gaugeData,
      title: {
        fontSize: 14
      },
      detail: {
        width: 40,
        height: 14,
        fontSize: 14,
        color: '#fff',
        backgroundColor: 'inherit',
        borderRadius: 3,
        formatter: '{value}%'
      }
    }
  ]
                    

}
                  )
    
  
  
      //设置回调函数 ，让页面隔一段时间刷新一下
      setTimeout(()=>{
          self.drawCharts()
      },5000)
        });
  
              
          },
          
          
      }
  
  }
  </script>
  
  <style>
  
  </style>