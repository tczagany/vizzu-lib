import { data_3 } from '../../../test_data/music_industry_history_1.mjs';

const testSteps = [
    chart => {
        chart.on('plot-axis-label-draw', event => {
            let year = parseFloat(event.data.text);
            if (!isNaN(year) && year % 5 != 0)
                event.preventDefault();
        });
            return chart.animate(
            {
            data: data_3,
            config: {
                channels: {
                    x: 'Year',
                    y: ['Format', 'Revenue [m$]'],
                    color: 'Format'
                },
                title: 'Range area chart',
                geometry: 'area',
                align: 'min',
                legend: null
            },
            style: {
                plot: {
                    marker: { colorPalette: '#f2b82dFF #00000000 #00000000' }
                }
            }
        });
    },
    chart => chart.feature('tooltip',true)
];

export default testSteps;