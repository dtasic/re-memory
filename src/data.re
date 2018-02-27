type tileStatus =
  | Hidden
  | Visible
  | Solved;

type tile = {
  id: int,
  sport: string,
  status: tileStatus
};

let rawtiles = [
  {id: 1, sport: "baseball", status: Hidden},
  {id: 2, sport: "basketball", status: Hidden},
  {id: 3, sport: "bowling", status: Hidden},
  {id: 4, sport: "football", status: Hidden},
  {id: 5, sport: "hockey", status: Hidden},
  {id: 6, sport: "pool", status: Hidden},
  {id: 7, sport: "rugby", status: Hidden},
  {id: 8, sport: "tennis", status: Hidden},
  {id: 9, sport: "baseball", status: Hidden},
  {id: 10, sport: "basketball", status: Hidden},
  {id: 11, sport: "bowling", status: Hidden},
  {id: 12, sport: "football", status: Hidden},
  {id: 13, sport: "hockey", status: Hidden},
  {id: 14, sport: "pool", status: Hidden},
  {id: 15, sport: "rugby", status: Hidden},
  {id: 16, sport: "tennis", status: Hidden}
];