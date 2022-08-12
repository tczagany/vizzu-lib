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
                    record.Format == 'DVD' ||
                    record.Format == 'Other' ||
                    record.Format == 'Tapes' ||
                    record.Format == 'Download' ||
                    record.Format == 'Streaming' ||
                    record.Format == 'Cassette' ||
                    record.Format == 'Vinyl' ||
                    record.Format == 'CD'
            }),
            config: {
                channels: {
                    x: { set: ['Format', 'Revenue [m$]'], range: { min:'0%', max:'110%'} },
                    y: { set: 'Year' },
                    color: { set: 'Format' }
                },
                title: 'Polar Stream Graph',
                geometry: 'area',
                coordSystem: 'polar',
                align: 'center',
                split: true
            },
            style: {
                plot: {
                    paddingLeft: '0em', 
                    paddingRight: '12.42em',
                    yAxis: {
                        label: { paddingRight: '0.8em' },
                        color: '#ffffff00'
                    },
                    xAxis: {
                        title: { paddingTop: '2.4em' },
                        label: { color: '#ffffff00' },
                        interlacing: { color: '#ffffff00' }
                    }
                }
            }
        });
    },
    chart => chart.feature('tooltip',true)
];

export default testSteps;