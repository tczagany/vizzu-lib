import { data } from '../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                record.Country == 'Austria' ||
                record.Country == 'Belgium' ||
                record.Country == 'Bulgaria' ||
                record.Country == 'Cyprus' ||
                record.Country == 'Czechia' ||
                record.Country == 'Denmark' ||
                record.Country == 'Estonia' ||
                record.Country == 'Greece' ||
                record.Country == 'Germany' ||
                record.Country == 'Spain' ||
                record.Country == 'Finland' ||
                record.Country == 'France' ||
                record.Country == 'Croatia' ||
                record.Country == 'Hungary'
        }),
        config: {
            channels: {
                x: { set: ['Year', 'Country' ] },
                y: { set: ['Value 2 (+)'] },
                color: { set: 'Country' }
            },
            title: 'Groupped Column',
            geometry: 'rectangle',
            orientation: 'horizontal',
            legend: null
        },
        style: {
            plot: {
                paddingLeft: '8em',
                yAxis: {
                    label: {
                       paddingRight: '0.8em',
                        fontSize: 13
                    }
                }
            }
        }
    }),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '110%' }  },
                color: { set: 'Country' }
            },
            title: 'Stacked Column / Sum(?) sub-elements',
            geometry: 'rectangle',
            orientation: 'horizontal',
            split: false,
            legend: 'color'
        },
        style: {
            plot: {
                paddingLeft: '0em'
            }
        }
    },
    {
        style: {
            delay: 0.6,
            duration: 1.4,
        },
         legend: {
             delay: 0.6,
             duration: 1.4,
         },
         x: {
             delay: 1,
             duration: 1,
//                easing: 'ease-out'
         }, 
         y: {
             delay: 0,
             duration: 1,
//             easing: 'cubic-bezier(.39,0,.35,.99)'
         }
     }
 ),
 chart => chart.animate({
         config: {
            channels: {
                x: { set: ['Year', 'Country' ] },
                y: { set: ['Value 2 (+)'] },
                color: { set: 'Country' }
            },
            title: 'Groupped Column / Comparison(?), Group(?)',
            geometry: 'rectangle',
            orientation: 'horizontal',
            legend: null
        },
        style: {
            plot: {
                paddingLeft: '8em',
                yAxis: {
                    label: {
                       paddingRight: '0.8em',
                        fontSize: 13
                    }
                }
            }
        }
    }),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: ['Country','Year', 'Value 2 (+)'], range: { min: '0%', max: '110%' }  },
                color: { set: 'Country' }
            },
            title: 'Stacked Column / Aggregate(?), Sum(?), - Discrete(?) elements',
            geometry: 'rectangle',
            orientation: 'horizontal',
            split: false,
            legend: 'color'
        },
        style: {
            plot: {
                paddingLeft: '0em'
            }
        }
    },
       {
            duration: 2,
        }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '110%' }  },
                color: { set: 'Country' },
                label: { set:['Value 2 (+)']}
            },
            title: 'Stacked Column / Aggregate(?), Sum(?), - Discrete(?) elements',
            geometry: 'rectangle',
            orientation: 'horizontal',
            split: false
        }
    },
    {
         delay: 0, duration: 0.2,
     }
 ),
 chart => chart.animate({
    config: {
        channels: {
            x: { set: null },
            y: { set: ['Country','Year', 'Value 2 (+)'], range: { min: '0%', max: '110%' }  },
            color: { set: 'Country' },
            label: { set:null }
        },
        title: 'Stacked Column / Aggregate(?), Sum(?), - Discrete(?) elements',
        geometry: 'rectangle',
        orientation: 'horizontal',
        split: false,
        legend: 'color'
    },
    style: {
        plot: {
            paddingLeft: '0em'
        }
    }
},
   {
    delay: 2, duration: 0.2,
    }
),
chart => chart.animate({
        config: {
            channels: {
                x: { set: ['Year', 'Country' ] },
                y: { set: ['Value 2 (+)'] },
                color: { set: 'Country' }
            },
            title: 'Groupped Column / Drill down(?), + Discrete(?), Groupped(?)',
            geometry: 'rectangle',
            orientation: 'horizontal',
            legend: null
        },
        style: {
            plot: {
                paddingLeft: '8em',
                yAxis: {
                    label: {
                       paddingRight: '0.8em',
                        fontSize: 13
                    }
                }
            }
        }
    }),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '100%' }  },
                color: { set: 'Country' }
            },
            title: 'Stacked Percentage Column / Ratio%(?)',
            align: 'stretch',
            legend: 'color'
        },
        style: {
            plot: {
                paddingLeft: '0em'
            }
        }
    },
    {
        style: {
            delay: 0.6,
            duration: 1.4,
        },
         legend: {
             delay: 0.6,
             duration: 1.4,
         },
         x: {
             delay: 1,
             duration: 1,
//                easing: 'ease-out'
         }, 
         y: {
             delay: 0,
             duration: 1,
//             easing: 'cubic-bezier(.39,0,.35,.99)'
         }
     }
 ),

 chart => chart.animate({
     config: {
         channels: {
             x: { set: ['Year', 'Country'] },
             y: { set: 'Value 2 (+)', range: { min: '0%', max: '110%' } },
             color: { set: 'Country' }
         },
         title: 'Groupped Column / Comparison(?), Group(?)',
         align: 'min',
     },
     style: {
         plot: {
             paddingLeft: '0em'
         }
     }
 }
),


chart => chart.animate({
    config: {
        channels: {
            x: { set: null },
            y: { set: ['Country', 'Year', 'Value 2 (+)'], range: { min: '0%', max: '110%' } },
            color: { set: 'Country' }
        },
        title: 'Column / Aggregate(?), - Discrete(?)',
        align: 'min',
    },
    style: {
        plot: {
            paddingLeft: '0em'
        }
    }
}
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: null },
            y: { set: 'Value 2 (+)', range: { min: '0%', max: '110%' } },
            color: { set: null },
            label: { set: 'Value 2 (+)' }
        },
        title: 'Column / Total(?) Aggregate(?) - Discrete(?)',
        align: 'min',
        legend: null 
    },
    style: {
        plot: {
            paddingLeft: '8em'
        }
    }
}
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: null },
            y: { set: ['Country', 'Year', 'Value 2 (+)'], range: { min: '0%', max: '110%' } },
            color: { set: null },
            label: { set: null }
        },
        title: 'Column / Drill down(?), + Discrete(?)',
        align: 'min',
        legend: null
    },
    style: {
        plot: {
            paddingLeft: '8em'
        }
    }
},
{
   delay: 2, duration: 1
}
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: ['Year', 'Country'] },
            y: { set: 'Value 2 (+)', range: { min: '0%', max: '110%' } },
            color: { set: 'Country' }
        },
        title: 'Groupped Column / Comparison(?), Group(?)',
        align: 'min',
        legend: 'color'
    },
    style: {
        plot: {
            paddingLeft: '0em'
        }
    }
},
{
   delay: 0, duration: 2
}
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: ['Country', 'Year', 'Value 2 (+)'] },
            y: { set: null, range: { min: '0%', max: '100%' } },
            color: { set: 'Country' }
        },
        title: 'Stacked Bar / Sum(?)',
        align: 'min',
    },
    style: {
        plot: {
            paddingLeft: '0em'
        }
    }
},
{
    delay:2,
    style: {
        delay: 1,
        duration: 1,
    },
    legend: {
        delay: 1,
        duration: 1,
    },
    x: {
//      easing: 'ease-in',
        delay: 0,
        duration: 1
    }, 
    y: {
//      easing: 'ease-out',
        delay: 0.5,
        duration: 1
    }
}
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: ['Country', 'Value 2 (+)'] },
            y: { set: null, range: { min: '0%', max: '100%' } },
            color: { set: 'Country' },
            label: { set: 'Value 2 (+)' }
        },
        title: 'Stacked Bar / Sum(?)',
        align: 'min',
    },
    style: {
        plot: {
            paddingLeft: '0em',
            marker: {
                label: {
                    fontSize: 9
                }
            }
        }
    }
},
{
    delay:0, duration: 0.3

}
),

    chart => chart.feature('tooltip',true)
];

export default testSteps;