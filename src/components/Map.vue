<!-- 商家分布图表 -->
<template>
  <div class='com-container' >
    <div class='com-chart' ref='map_ref' ></div>
  </div>
</template>

<script>
  import purple from '../../public/static/theme/purple-passion.json'
import { getProvinceMapInfo } from '@/utils/map_utils'
export default {
  data () {
    return {
      chartInstance: null,
      allData: null,
      mapData: {},
      
geoCoordMap : [
//绿洲农业分布地区
{"name":"吐鲁番市","value":[89.18579,42.93505]},
{"name":"哈密市","value":[93.51452,42.82699]},
{"name":"库尔勒","value":[86.15528,41.76602]},
{"name":" 阿克苏市","value":[	80.26338,41.16754]},
{"name":"喀什市","value":[75.99379,39.46768]},
{"name":"和田市","value":[79.91353,37.11214]},
{"name":"博乐市","value":[82.0713,44.90052]},
{"name":"石河子市","value":[86.041075,44.305886]},
{"name":"塔城市","value":[	82.97892,46.74852]},
{"name":"阿勒泰市","value":[88.13913,47.8317]},
{"name":"敦煌市","value":[94.66159,40.14211]},
{"name":"果洛藏族自治州","value":[100.242143,34.4736]},
{"name":"海南藏族自治州","value":[100.619542,36.280353]},

]
    
}
    
  },

  mounted () {
    this.initChart()
  


  },
  destroyed () {

  },
  methods: {
    async initChart () {
      this.$echarts.registerTheme("purple",purple)//注册主题
      this.chartInstance = this.$echarts.init(this.$refs.map_ref, 'purple')


      // 获取中国地图的矢量数据--第二种方式，文件存放在本地，直接require本地json文件，经测试，两种效果都一样
      const appData = require('../json/xinjiang.json')
      console.log(appData)
      console.log(this.geoCoordMap)
      this.$echarts.registerMap('xinjiang', appData)
      //处理需要在地图上展示的数据
      const seriesArr=this.geoCoordMap.map(item=>{
          return{
              type:'effectScatter',
              name:item.name,
              data:item.value,
              coordinateSystem:'geo'
          }
      })
      console.log("seriesArr:")
      console.log(seriesArr)
      const initOption = {
        title: {
          text: '▎ 绿洲农业分布',
          left: 20,
          top: 20
        },
        visualMap: {
          min: 1111,
          max: 333332,
          realtime: false,
          calculable: true,
          inRange: {
            color: ['lightskyblue', 'yellow', 'orangered']
          }
        },



  
        legend: {
          left: '5%',
          bottom: '5%',
          orient: 'vertical'
        },
        // 在地图上绘制绿洲农业的位置
        series: [
          {
  
            type: 'map',
            map: 'xinjiang',// 这个是上面注册时的名字哦，registerMap（'这个名字保持一致'）
            label: {
              show: true
            },

            data: [
              { name: '吐鲁番市', value: 11111 },
              { name: '哈密市', value: 22221 },
              { name: '库尔勒市', value: 33333 },
              { name: '阿克苏地区', value: 22231 },
              { name: '喀什地区', value: 44441 },
              { name: '和田地区', value: 555525 },
              { name: '博乐市', value: 162475 },
              { name: '石河子市', value: 65454 },
              { name: '塔城市', value: 77771 },
              { name: '阿勒泰地区', value: 88884 },
              { name: '敦煌市', value: 99995 },

 
              // {name:"南海诸岛",itemStyle:{  
              //    normal:{opacity:0,label:{show:false},borderWidth:"0",borderColor:"#10242b",areaStyle:{color:'#10242b'}},
              // emphasis:{opacity:0,label:{show:false}}
              //     }
              // },

            ]
          },
        ]


     

      }
      this.chartInstance.setOption(initOption)
      
    },
    

  }
}
</script>

<style lang='less' scoped>
</style>