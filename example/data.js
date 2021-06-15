
const cat0 = [ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' ];
const cat1 = [ 'A', 'B', 'A', 'B', 'A', 'B', 'A', 'B', 'A', 'B' ];
const cat2 = [ '1930', '1940', '1950', '1960', '1970', '1980', '1990', '2000', '2010', '2020' ];
const cat3 = ['jan','feb','marc','apr','maj','jun','jul','aug','sept','okt'];
const cat4 = ['1','2','3','4','5','6','7','8','9','10'];
const cat5 = [ 'a0', 'b0', 'c0', 'd0', 'e0', 'f0', 'g0', 'h0', 'i0', 'j0' ];
const cat6 = [ 'a1', 'b1', 'c1', 'd1', 'e1', 'f1', 'g1', 'h1', 'i1', 'j1' ];
const val0 = [6.5,2.9,9.3,7.8,1.0,7.2,5.6,3.4,9.2,5.0];
const val1 = [5.6,9.2,3.9,8.7,0.1,2.7,6.5,4.3,2.9,0.5];

function cat(name, values) {
	return { name, type: 'categories', values };
}

function val(name, values) {
	return { name, type: 'values', values };
}

const data =
{
	series: [
		cat('cat0', cat0),
		cat('cat1', cat1),
		cat('cat2', cat2),
		cat('cat3', cat3),
		cat('cat4', cat4),
		cat('cat5', cat5),
		cat('cat6', cat6),
		val('val0', val0),
		val('val1', val1)
	]
}

export default data;