import { data } from '../../../test_data/funnel_data.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: data,
            config: {
                channels: {
                    x: { set: 'Num' }, 
                    y: { set: 'Phase' },
                    color: { set: 'Phase' },
                    label: { set: ['Phase', 'Num'] }
                },
                title: 'Funnel',
                geometry: 'area',
                align: 'center'
            },
            style: {
                plot: {
                    paddingLeft: '3.8em',
                    paddingRight: '8em',
/*                    marker:{
                        label:{
                            fontSize: '1.1em',
                            position:'left',
                            paddingTop: '1.65em',
                            format:'dimensionsFirst'
                        }},
                        */
                    yAxis: {
                        color: "#00000000",
                        label: { color: "#00000000" }
                    },
                    xAxis: {
                        color: "#00000000",
                        title: { color: "#00000000" },
                        label: { color: "#00000000" },
                        interlacing: { color: "#00000000" } 
                    }
                }
            }
        }
    ),
    chart => chart.feature('tooltip',true)
];

export default testSteps;