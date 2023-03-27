<!-- 针对/temppage这条路径现实出来 -->
<!-- 在这个组件中，通过子组件注册的方式，要显示出Temp.vue这个租件 -->
<template>
    <div class="com-page">
        <soilp></soilp>
    </div>
    
  </template>
  
  <script>
  // 配置Temp
  import Soil from '@/components/Soil'
  
  export default {
  data(){
      return{}
  },
  methods:{},
  components:{
      soilp:Soil
  }
  }
  </script>
  
  <style lang="less" scoped>
  
  </style>