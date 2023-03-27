<template>
    <div class="screen-container">
      <header class="screen-header">
        <div>
        <img src="/static/img/header_border_dark.png" alt="">
      </div>
      <span class="logo">
        19035206台耀强毕业设计
      </span>
      <span class="title">绿洲农业监控系统</span>

    
        <div class="title-right">
            
          <span class="datetime">{{ nowTime }}</span>
        </div>
      </header>
      <div class="screen-body">
        
        <section class="screen-left">
            <div id="left-top" >
            <!-- 销量趋势图表 -->
            <Temp ref="trend"></Temp>
          </div>

          <div class="left-bottom2" >
            <!-- 商家销售金额图表 -->
            <NPK ref="seller"></NPK>   
          </div>
        </section>

        <section class="screen-middle">
          <div id="middle-top" >
            <!-- 商家分布图表 -->
            <Map ref="map"></Map>
           
          </div>

          <div id="middle-bottom" :class="[fullScreenStatus.rank ? 'fullscreen' : '']">
            <!-- 地区销量排行图表 -->
            <Rank ref="rank"></Rank>
           
          </div>
        </section>

        <section class="screen-right">
          <div id="right-top" >
            <!-- 热销商品占比图表 -->
            <Trend ref="hot"></Trend>
            
          </div>

          <div id="right-bottom" >
            <!-- 库存销量分析图表 -->
            <Stock ref="stock"></Stock>
                  
          </div>

        </section>
      </div>
    </div>
  </template>
  
  <script>
  import Temp from '@/components/Temp.vue'
  import Map from '@/components/Map.vue'
  import NPK from '@/components/NPK.vue'
  import Seller from '@/components/Picture.vue'
  import Stock from '@/components/Soil.vue'
  import Trend from '@/components/SoilHum.vue'
//   import { mapState } from 'vuex'
//   import { getThemeValue } from '@/utils/theme_utils'
  export default {
    created(){
      // this.$socket.registerCallBack('fullScreen', this.recvData)
      this.nowTimes()
    },
    // destroyed(){
    //   this.$socket.unregisterCallBack('fullScreen')
    // },
    data() {
      return {
        nowTime: '',
        fullScreenStatus: {
          trend: false,
          seller: false,
          map: false,
          rank: false,
          hot: false,
          stock: false
        }
      }
    },
    methods: {
     
      //显示当前时间（年月日时分秒）
      timeFormate(timeStamp) {
        let year = new Date(timeStamp).getFullYear();
        let month = new Date(timeStamp).getMonth() + 1 < 10 ? "0" + (new Date(timeStamp).getMonth() + 1) : new Date(timeStamp).getMonth() + 1;
        let date = new Date(timeStamp).getDate() < 10 ? "0" + new Date(timeStamp).getDate() : new Date(timeStamp).getDate();
        let hh = new Date(timeStamp).getHours() < 10 ? "0" + new Date(timeStamp).getHours() : new Date(timeStamp).getHours();
        let mm = new Date(timeStamp).getMinutes() < 10 ? "0" + new Date(timeStamp).getMinutes() : new Date(timeStamp).getMinutes();
        let ss = new Date(timeStamp).getSeconds() < 10 ? "0" + new Date(timeStamp).getSeconds() : new Date(timeStamp).getSeconds();
        let week = new Date(timeStamp).getDay();
        let weeks = ["日", "一", "二", "三", "四", "五", "六"];
        let getWeek = "星期" + weeks[week];
        this.nowTime = year + "年" + month + "月" + date + "日" + " " + hh + ":" + mm + ':' + ss + getWeek;
      },
      nowTimes() {
        this.timeFormate(new Date());
        setInterval(this.nowTimes, 1000);
        this.clear()
      },
      clear() {
        clearInterval(this.nowTimes)
        this.nowTimes = null;
      },
    },
  
    components: {
      Temp,
      Map,
      NPK,
      Seller,
      Stock,
      Trend
    },
  }
  </script>
  <style lang="less" scoped>
  .fullscreen {
    position: fixed ;
    top: 0 ;
    left: 0 ;
    width: 100% ;
    height: 100% ;
    margin: 0 ;
    z-index: 100;
  }
  
  .screen-container {
    width: 100%;
    height: 100%;
    padding: 0 20px;
    background-color: #161522;
    color: #fff;
    box-sizing: border-box;
  }
  
  .screen-header {
    width: 100%;
    height: 64px;
    font-size: 20px;
    position: relative;
  
    > div {
      img {
        width: 100%;
      }
    }
  
    .title {
      position: absolute;
      left: 50%;
      top: 50%;
      font-size: 20px;
      transform: translate(-50%, -50%);
    }
  
    .title-right {
      display: flex;
      align-items: center;
      position: absolute;
      right: 0px;
      top: 50%;
      transform: translateY(-80%);
    }
  
    .qiehuan {
      width: 28px;
      height: 21px;
      cursor: pointer;
    }
  
    .datetime {
      font-size: 15px;
      margin-left: 10px;
    }
  
    .logo {
      position: absolute;
      left: 0px;
      top: 50%;
      transform: translateY(-80%);
  
      img {
        height: 35px;
        width: 128px;
      }
    }
  }
  
  .screen-body {
    width: 100%;
    height: 100%;
    display: flex;
    margin-top: 10px;
  
    .screen-left {
      height: 100%;
      width: 27.6%;
  
      #left-top {
        height: 46%;
        position: relative;
      }
  
      .left-bottom2 {
        height: 38%;
        margin-top: 25px;
        position: relative;
      }
    }
  
    .screen-middle {
      height: 100%;
      width: 41.5%;
      margin-left: 1.6%;
      margin-right: 1.6%;
  
      #middle-top {
        width: 100%;
        height: 90%;
        position: relative;
        margin-bottom: 50px;
      }
  
      #middle-bottom {
        margin-top: 25px;
        width: 100%;
        height: 28%;
        position: relative;
      }
    }
  
    .screen-right {
      height: 100%;
      width: 27.6%;
  
      #right-top {
        height: 46%;
        position: relative;
      }
  
      #right-bottom {
        height: 38%;
        margin-top: 25px;
        position: relative;
      }
    }
  }
  
  .resize {
    position: absolute;
    right: 20px;
    top: 20px;
    cursor: pointer;
  }
  </style>