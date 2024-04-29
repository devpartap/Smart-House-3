<template>
<v-app>

    <!-- ------ Main Title Bar ------ -->
    <v-app-bar :elevation="1" :height="60">

        <template v-slot:prepend>
            <v-app-bar-nav-icon @click="drawer = !drawer">
                <v-icon size="x-large">mdi-menu</v-icon>
            </v-app-bar-nav-icon>
        </template>

        <v-app-bar-title >{{$hp[$global.active_room].name}}</v-app-bar-title>

        <v-badge :content="5" style="margin-right: 20px;">
            <v-icon size="x-large">mdi-cog-outline</v-icon>
        </v-badge>
    </v-app-bar>

    <!-- ------ Main Navigation Bar ------ -->
    <v-navigation-drawer v-model="drawer" location="left"
        :width="360" temporary >
        
        <v-card style="padding-top: 15px;" flat>

            <v-card-title class="font-weight-bold">
                HOUSE LAYOUT
            </v-card-title>

            <v-card-subtitle>
                <v-divider :thickness="3" color="black"></v-divider><br>
            </v-card-subtitle>

            <v-expansion-panels v-for="floor in Object.keys($hp.layout).reverse()" :key="floor"
                                flat>

                <v-expansion-panel :title="m_getFloorName(floor)" variant="popout" eager ripple>
                    <v-expansion-panel-text>

                        <v-list v-for="room in $hp.layout[floor]" 
                            density="compact">
                            <v-list-item @click="changeActiveRoom(room)">
                                {{ room }}
                            </v-list-item>
                        </v-list>

                    </v-expansion-panel-text>
                </v-expansion-panel>

            </v-expansion-panels>
        </v-card>


        <v-card style="padding-top: 15px;" flat>

            <v-card-title class="font-weight-bold">
                ACTIONS
            </v-card-title>

            <v-card-subtitle>
                <v-divider :thickness="3" color="black"></v-divider><br>
            </v-card-subtitle>

        </v-card>
    </v-navigation-drawer>

    
    <!-- ------ Main Page Components ------ -->
    <v-main style="height: 100%;">

    <div style="margin-left: 20px;margin-right: 20px;align-items: center;">
        <br>
        <v-table density="comfortable">


            <tbody>
                <tr v-for="device in $hp[$global.active_room].devices" :key="device.id" >
                    <td class="text-h6 font-weight-bold" style="color: #424242; ">
                        {{ device.name }}
                    </td>

                    <td >
                        <v-switch style="float: right;" color="primary" hide-details inset
                                  v-model="device.state" @click="sendSignal(device.id)"></v-switch>
                    </td>
                </tr>
            </tbody>
        </v-table>
    </div>


    </v-main>


    </v-app>
</template>

<script setup>

import {ref,reactive,inject} from 'vue'

const $hp = ref(inject('$hp'))
const $global = reactive(inject('$g'))

const drawer = ref(false)

let listColors = {
    "light":"green",
    "fan":"blue",
    "strip":"yellow",
    "switch":"gery"
}


//   --- Getting Current Roomdata

function changeActiveRoom(_room_id_)
{
    $global.active_room = _room_id_
}

function sendSignal(_device_id_)
{
    console.log(_device_id_)
}

function m_getFloorName(_floor_)
{
    if(_floor_ == 0)
    {
        return "Ground Floor"
    }
    if(_floor_ == 1)
    {
        return "1st Floor"
    }
    if(_floor_ == 2)
    {
        return "2nd Floor"
    }
    if(_floor_ == 3)
    {
        return "3rd Floor"
    }
    else
    {z
        return _floor_ + "th Floor"
    }
}




const xhr = new XMLHttpRequest();
function sendxhr()
{
    xhr.open('GET',($global.master_url))
    xhr.send()
}

xhr.onload = () => {
    console.log(xhr.response)
    setTimeout(sendxhr, 800)
}


sendxhr()





</script>