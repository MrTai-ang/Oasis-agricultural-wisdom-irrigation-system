<template>
  <div class="com-container" >
    <div class="com-chart" id="myChart4"  >
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
             let myChart = this.$echarts.init(document.getElementById("myChart4"),'purple');
            //获取数据
            self.$axios.get('temprouter')
            .then(res=>{
                const all_data=res.data
                console.log(all_data)
                const xdata=[]
                const y_humidity=[]
                const y_temperature=[]
                const y_light=[]

                
                for(var i=0;i<all_data.length;i++)
                {
                    y_humidity.push(all_data[i].humidity/2)
                    y_temperature.push(all_data[i].temperature)
                    y_light.push(all_data[i].light/20.0)
                    //处理时间戳，截取小时分钟
                    var nowtime=all_data[i].time
                    var nowtime2=nowtime.substring(11,16)
                    xdata.push(nowtime2)
                }
                console.log('y_humidity:',y_humidity)
                console.log('y_temperature:',y_temperature)
                console.log('y_light:',y_light)
                console.log('xdata:',xdata)
              
              // 测试
                console.log('打印土壤数据')
                self.$axios.get('soilrouter')
            .then(res=>{
              console.log(res.data)
            })
            console.log('打印土壤npk数据')
                self.$axios.get('npkrouter')
            .then(res=>{
              console.log(res.data)
            })
            console.log('打印土壤湿度数据')
                self.$axios.get('humrouter')
            .then(res=>{
              console.log(res.data)
            })

                myChart.setOption({
                color: ['#80FFA5', '#00DDFF', '#37A2FF', '#FF0087', '#FFBF00'],
  title: {
    text: ' ┃ 环境参数'
  },
  tooltip: {
    trigger: 'axis',
    axisPointer: {
      type: 'cross',
      label: {
        backgroundColor: '#6a7985'
      }
    }
  },
  legend: {
    data: ['温度', '湿度/2','光强/20']
  },
  toolbox: {
    feature: {
      saveAsImage: {}
    }
  },
  grid: {
    left: '3%',
    right: '4%',
    bottom: '3%',
    containLabel: true
  },
  xAxis: [
    {
      type: 'category',
      boundaryGap: false,
      data: xdata
    }
  ],
  yAxis: [
    {
      type: 'value'
    }
  ],
  series: [
    {
      name: '温度',
      type: 'line',
      stack: 'Total',
      smooth: true,
      lineStyle: {
        width: 0
      },
      showSymbol: false,
      areaStyle: {
        opacity: 0.8,
        color: new echarts.graphic.LinearGradient(0, 0, 0, 1, [
          {
            offset: 0,
            color: 'rgb(128, 255, 165)'
          },
          {
            offset: 1,
            color: 'rgb(1, 191, 236)'
          }
        ])
      },
      emphasis: {
        focus: 'series'
      },
      data: y_temperature
    },
        {
      name: '湿度/2',
      type: 'line',
      stack: 'Total',
      smooth: true,
      lineStyle: {
        width: 0
      },
      showSymbol: false,
      areaStyle: {
        opacity: 0.8,
        color: new echarts.graphic.LinearGradient(0, 0, 0, 1, [
          {
            offset: 0,
            color: 'rgb(55, 162, 255)'
          },
          {
            offset: 1,
            color:  'rgb(116, 21, 219)'
          }
        ])
      },
      emphasis: {
        focus: 'series'
      },
      data: y_humidity
    },
    {
      name: '光强/20',
      type: 'line',
      stack: 'Total',
      smooth: true,
      lineStyle: {
        width: 0
      },
      showSymbol: false,
      label: {
        show: true,
        position: 'top'
      },
      areaStyle: {
        opacity: 0.8,
        color: new echarts.graphic.LinearGradient(0, 0, 0, 1, [
          {
            offset: 0,
            color: 'rgb(255, 191, 0)'
          },
          {
            offset: 1,
            color: 'rgb(224, 62, 76)'
          }
        ])
      },
      emphasis: {
        focus: 'series'
      },
      data: y_light
    }

]    
            })

  


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