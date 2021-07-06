
const testData = {
    series: [
        {
            name: 'Colors',
            type: 'categories',
            values: ['red', 'green', 'blue']
        },
        {
            name: 'Val',
            type: 'values',
            values: [ 3, 5, 4 ]
        }
    ]
};

const testSteps = [
    { task: chart => chart.animate(
        {
            data: testData,
            descriptor : {
                channels: {
                    x: { attach: [ 'Colors'] },
                },
                title: null,
                legend: null,
            }
        }),
        ref: 'c79f4aebf290438567ad121bd92059678475fef0fcc3e257a944219a029449cf'},
    { task: chart => chart.animate(
        {
            data: {
                filter: record => record.Colors != 'blue'
            },
            descriptor : {
                channels: {
                    x: { detach: [ 'Colors'] },
                    y: { attach: [ 'Colors' ]}
                },
            }
        }),
        ref: '57cbea907308db40ca5797f918ac467e0b2e518d30177a9999f99521570f83f0'},
    { task: chart => chart.animate(
        {
            descriptor : {
                channels: {
                    color: { attach: [ 'Colors' ]}
                }
            }
        }),
        ref: '26d4062ff2037d1c8dd101a163afd4a35d5e6a945ec03a9c8faeb7b691c30fed'},
    { task: chart => chart.animate(
        {
            data: {
                filter: null
            },
            descriptor : {
                channels: {
                    color: { detach: [ 'Colors' ]},
                    lightness: { attach: [ 'Colors' ]}
                }
            }
        }),
        ref: '8f02437e355e3edb7dd0d0712526f53718707d448f214a0d08ec29598494c1a6'},
    { task: chart => chart.animate(
        {
            descriptor : {
                channels: {
                    lightness: { detach: [ 'Colors' ]},
                    label: { attach: [ 'Colors' ]}
                }
            }
        }),
        ref: '2cd9496665e7f04d88c65dd23552faed7d0a2bcb3eacf7d1a67f85f1462694c3'},
]


export default { testData, testSteps }
