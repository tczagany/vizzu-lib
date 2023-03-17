import { data_8 } from '../../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_8,

        config: {
            channels: {
                color: 'Country' ,
                size: 'Value 2 (+)',
                label: 'Value 2 (+)'
            },
            title:'Title',
            geometry: 'circle'
        }
    }
),

    chart => chart.animate({
        config: {
            channels: {
                size: ['Year', 'Value 2 (+)']
            },
            title:'Title',
            geometry: 'circle'
        }
    }
),
 
    chart => chart.animate({
        config: {
            channels: {
                x: 'Value 1 (+)',
                y: 'Value 3 (+)',
                noop: 'Year',
                size: 'Value 2 (+)',
                label: null
            },
            title:'Title'
        }
    }
),
    chart => chart.feature('tooltip',true)
];

export default testSteps;