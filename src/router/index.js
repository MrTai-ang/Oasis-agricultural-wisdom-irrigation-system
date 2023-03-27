import Vue from 'vue'
import VueRouter from 'vue-router'
import TempPage from '@/views/TempPage'
import MapPage from '@/views/MapPage'
import SoilPage from '@/views/SoilPage'
import NPKPage from '@/views/NPKPage'
import SoilHumPage from '@/views/SoilHumPage'
import PicturePage from '@/views/PicturePage'
import ScreenPage from '@/views/ScreenPage'

Vue.use(VueRouter)

const routes = [
  {
    path:'/',
    redirect:'/screen'
  },
  {

    path:'/screen',
    component:ScreenPage
  },
  {
        path: '/temppage',
        component: TempPage
  },
  {
    path: '/mappage',
    component: MapPage
},
{
  path: '/soilpage',
    component: SoilPage
},
{
  path: '/npkpage',
  component: NPKPage
},
{
  path: '/soilhumpage',
  component: SoilHumPage
},
{
  path: '/picturepage',
  component: PicturePage
}
//最后一个这里不要加逗号，奇怪的报错，加了以后就加载不出来路径

  
]

const router = new VueRouter({
  routes
})

export default router
