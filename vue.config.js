const { defineConfig } = require('@vue/cli-service')
module.exports = defineConfig({
  transpileDependencies: true
})

module.exports={
  devServer:{
    
    port:8080,//配置端口号
    // open:true//自动打开
  }
}