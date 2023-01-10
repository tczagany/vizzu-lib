import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
            ['AT', 'BE', 'CY', 'DE', 'DK', 'EE', 'EL', 'ES', 'FI', 'FR', 'IT', 'NL', 'SE']
            .includes(record.Country_code)
    }),
        config: {
            channels: {
                x: 'Year',
                y: ['Value 2 (+)', 'Country'],
                color: 'Country'
            },
            title: 'Stacked Area Chart',
            geometry: 'area'
        }
    }),
    
    chart => chart.animate({
        config: {
            title: '100% Stacked Area Chart',
            align: 'stretch'
        }
    }),

    chart => chart.animate({
        config: {
            channels: {
                y: {
                    /* Making the chart elements fill the whole of the y-axis
                    as the default value is now 110% */
                    range: {
                        max: '100%' 
                    }
                },
            },
            title: 'Trellis (Splitted) Area Chart',
            align: 'min',
            split: true
        }
    })
];

export default testSteps;