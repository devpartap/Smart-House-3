<template>
<v-app>

    <!-- ------ Main Title Bar ------ -->
    <v-app-bar :elevation="1" :height="60">

        <template v-slot:prepend>
            <v-app-bar-nav-icon @click="b_drawer = !b_drawer">
                <v-icon size="x-large">mdi-menu</v-icon>
            </v-app-bar-nav-icon>
        </template>

        <v-app-bar-title >{{$hp[$global.active_room].name}}</v-app-bar-title>

        <v-badge :content="5" style="margin-right: 20px;">
            <v-icon size="x-large">mdi-cog-outline</v-icon>
        </v-badge>
    </v-app-bar>


    <!-- ------ Main Navigation Bar ------ -->
    <v-navigation-drawer v-model="b_drawer" location="left"
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

                <v-expansion-panel :title="getFloorName(floor)" variant="popout" eager ripple>
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


            <!-- <tbody v-show="b_ready_to_use"> -->
            <tbody>
                <tr v-for="device in $hp[$global.active_room].devices" :key="device.id"  >
                    <td class="text-h6 font-weight-bold" style="color: #424242; ">
                        {{ device.name }}
                    </td>

                    <td >
                        <v-switch style="float: right;" :color="(device.state) ? 'success' : '' " hide-details inset :loading="switch_loading == device.id" 
                        v-model="device.state" @change="sendDeviceState($global.active_room,device.id,device.state)"></v-switch>
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

const b_drawer = ref(false)
const b_ready_to_use = ref(false);

const switch_loading = ref(0.0)



//   --- UI Interactive Functions ---

function changeActiveRoom(_room_id)
{
    $global.active_room = _room_id
}

function sendSignal(_device_id)
{
    console.log(_device_id)
}

function getFloorName(_floor)
{
    if(_floor == 0)
    {
        return "Ground Floor"
    }
    if(_floor == 1)
    {
        return "1st Floor"
    }
    if(_floor == 2)
    {
        return "2nd Floor"
    }
    if(_floor == 3)
    {
        return "3rd Floor"
    }
    else
    {
        return _floor + "th Floor"
    }
}

function toggleSwitch()
{

}

//   --- Functions

//   --- Network Functions ---

const socket = new WebSocket('ws://192.168.29.167:0080/')

socket.addEventListener("message", (event) => {
    processServerMessage(event.data);
});

socket.onopen = () => {
    console.log("Handshake complete!")   
    askFloorStatus()
}

socket.onclose = (msg) => {
    console.log(msg);
    console.log("Connection Closed!")
}

socket.onerror = (err) => {
    console.log(err);
}

function processServerMessage(data)
{
    if(data[0] == 'D')
    {
        let index = data.charCodeAt(1).toString() + "." + (data.charCodeAt(2)+1).toString();       

        console.log("here");
        console.log(index);
        
        $hp.value[index].devices[data.charCodeAt(4)].state = Boolean(data.charCodeAt(5))
        console.log($hp.value[index].devices[data.charCodeAt(4)].state);
    }
}


function sendDataOnWS(_data)
{
    return new Promise(function(resolve,reject) {
        socket.send(_data);
        socket.onmessage = (receved_Data) => {
            
            if(receved_Data.data[0] == 'N')
            {
                reject(receved_Data)
            }
            else
            {
                resolve(receved_Data)
            }
        }
    });
   
}

async function sendDeviceState(_RoomID,_DeviceID,_state)
{
    // TODO: IMPROVE THIS COMUNICATION
    try{
        switch_loading.value = _DeviceID

        let responce = await sendDataOnWS(`U ${_RoomID}.${_DeviceID}.${Number(_state)}`)
        switch_loading.value = 0
        console.log(responce.data)
        // activecolor = "success"

    } 
    catch (error) {
        console.log("ERROR FROM SERVER :- " + error);
        
    }
}

async function askFloorStatus()
{
    try{
        let responce = await sendDataOnWS(`F${Math.trunc($global.active_room)}`)
        let i = 0;

        $hp.value.layout[Math.trunc($global.active_room)].forEach((ele) => {
            $hp.value[ele].devices.forEach((div) => {

                div.state = Boolean(responce.data[i] == '1');
                i++;
            })
        })
        console.log(responce.data)
        console.log($hp.value['0.1'].devices) // please keep it in yr fkin mind i spend whole day debugging and found this was manual

        b_ready_to_use.value = true;

    } 
    catch (error) {
        console.log("ERROR FROM SERVER :- " + error);
    }
}

 

</script>