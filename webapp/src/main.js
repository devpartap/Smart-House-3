import { VueElement, createApp } from 'vue'
import App from './App.vue'

import './assets/style.css'


// router 
import { createRouter, createWebHistory } from 'vue-router'

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      name: 'main',
      component: () => import("./pages/main.vue")
    }
  ]
})

// Vuetify
import 'vuetify/styles'
import '@mdi/font/css/materialdesignicons.css'
import { createVuetify } from 'vuetify'
import * as components from 'vuetify/components'
import * as directives from 'vuetify/directives'

const vuetify = createVuetify({
  components,
  directives,
})

const app = createApp(App)

// profiling 
// NOTE :  IMPROVE THIS PROFILING SHIT
// - in house profile, room id is in format of (floor no) . (room no) [eg 1.2 which means it's room 2 on first floor]
// - in house profile, device id is i frmat of (worker id) . (switch no) [eg 2.2 which means it's connected to 2nd worker and to it's 2nd switch ]
import house_profile from '../houseprofile.json'
console.log(house_profile)


app.provide('$hp', house_profile)
app.provide('$g', {
  active_room: 1.1,
})

// app setup
app.use(router)
app.use(vuetify)
app.mount('#app')
