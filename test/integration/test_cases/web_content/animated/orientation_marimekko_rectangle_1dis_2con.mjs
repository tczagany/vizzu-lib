import { data_4 } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate({
        data: data_4,
        config: {
            channels: {
                x: ['Country', 'Value 2 (+)'],
                y: ['Country', 'Value 3 (+)'],
                color: 'Country',
                label: 'Country',
                lightness: 'Country'
            },
            title: 'Marimekko Chart',
            align: 'stretch',
            orientation: 'horizontal'
        }
    }),

    chart => chart.animate({
        config: {
            title: 'Marimekko with Other Orientation I',
            orientation: 'vertical'
        }
    })
];

export default testSteps;