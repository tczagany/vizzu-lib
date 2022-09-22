import { data } from '../../../test_data/music_industry_history_1.mjs';

const testSteps = [
    chart => {
        chart.on('plot-axis-label-draw', event => {
            let year = parseFloat(event.data.text);
            if (!isNaN(year) && year % 5 != 0)
                event.preventDefault();
        });
            return chart.animate(
            {
            data: Object.assign(data, {
                filter: record =>
        //            record.Format == 'DVD' ||
        //            record.Format == 'Other' ||
        //            record.Format == 'Tapes' ||
        //            record.Format == 'Download' ||
                    record.Format == 'Streaming' ||
                    record.Format == 'Cassette' ||
                    record.Format == 'Vinyl' 
        //            record.Format == 'CD'
            }),
            config: {
                channels: {
                    x: { set: 'Year' },
                    y: { set: ['Format', 'Revenue [m$]'] },
                    color: { set: 'Format' }
                },
                title: 'Range area chart',
                geometry: 'area',
                align: 'min',
                legend: null
            },
            style: {
                plot: {
                    paddingLeft: '8em',
                    marker: {
                        colorPalette: '#f2b82dFF #00000000 #00000000'
                    },
                    yAxis: {
                        label: {
                           paddingRight: '0.8em'
                        }
                    },
                    xAxis: {
                        label: {
                           paddingTop: '0.8em'
                        }
                    }
                }
            }
        });
    },
    chart => chart.feature('tooltip',true)
];

export default testSteps;