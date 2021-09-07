import {data} from './tutorial_data.js';
import Vizzu from 'https://vizzu-lib-main.storage.googleapis.com/lib/vizzu.js';


  let chart = new Vizzu('myVizzu');

  let slider = document.getElementById("myRange");
  
  slider.oninput = (e)=>
  {
    let t = e.target.value;
    chart.animation.pause();
    chart.animation.seek(t/10 + '%');
    
  };

 // 9. Charts without coordinates, noop channel
  chart.initializing.then( 
 // Induló állapot
    chart => chart.animate({ 
    data: data,
    config: {
      channels: {
        y: { attach: ['Popularity','Types'] },
        x: { attach: ['Genres'] },
        color:{set:['Types']},
        label:{set: ['Popularity']},
      },
      title:'Charts without coordinates = no data on the axes'
    }
    }))
    

// 9.1 - treemap
    .then(chart =>  chart.animate({
        config: {
          channels:{
            y:{set: null},
            x:{set: null},
            size:{attach: ['Genres','Popularity',]},
          },
          title:'Treemap'
        },
      })
    )

// 9.2 - bubble chart
    .then(chart =>  chart.animate({
      config: {
        geometry: 'circle',
        title:'Bubble chart - stacked'
        }
      },
    )
  )

// 9.3 - bubble chart
.then(chart =>  chart.animate({
  config: {
    channels:{
      size:{detach: ['Genres']},
      noop:{set: ['Genres']}
    },
    title:'Bubble chart - grouped - using the noop channel'
    }
  },
)
)

.


        catch((err) => {
          console.log(err);
        });

 
