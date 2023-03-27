import Vue from 'vue'
import App from './App.vue'
import router from './router'
import store from './store'
import './assets/css/global.less'
import axios from 'axios'
import * as echarts from 'echarts'
Vue.prototype.$echarts = echarts
//请求基准路径,后台服务器
axios.defaults.baseURL='http://127.0.0.1:3000/api/user'
// 将axios挂载到vue原型对象上
// 在别的组件中 this.$http就可以通过axios进行ajax请求
// Vue.prototype.$http = axios
//就得这样配置，上面那个不行
Vue.prototype.$axios = axios

//将全局echarts对象挂载到vue的原型对象上
//在别的组件中 this.$echarts就可以使用
Vue.config.productionTip = false
// Vue.prototype.$echarts = window.echarts
new Vue({
  router,
  store,
  render: h => h(App)
}).$mount('#app')
